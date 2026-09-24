#include <tt/ClientFactory.h>
#include <tt/Client.h>

tt::ClientFactory::ClientFactory(int maxClients): 
maxClients(maxClients), tp(maxClients)
{
    // If the JVM shuts down, close the server and wait for it to shut down.
}

/**
 * Creates a new client factory with no upper limit on the number of clients
 * that may run simultaneously.
 */
tt::ClientFactory::ClientFactory():
ClientFactory(std::thread::hardware_concurrency())
{
}

std::string tt::ClientFactory::toString() const
{
    std::string string = "[Client Factory: ";
    if(maxClients > 0)
        string += std::to_string((waiting.size() + running.size())) + "/" + std::to_string(maxClients) + " clients; ";
    return string + std::to_string(waiting.size()) + " waiting; " + std::to_string(running.size()) + " running]";
}

void tt::ClientFactory::execute()
{
    // Notify factory that it has started.
    onStart();
    // Interrupting skips lifecycle methods.
    bool interrupted = false;
    try {
        // Create first client.
        createNewClient();
        // Run operations until closed.
        while (!closed) {
            std::unique_ptr<Operation> ptr = nullptr;
            queue.Pop(ptr);
            ptr->execute();
        }
        // Notify factory that it has closed.
        try {
            onClose();
        }
        catch (...) {
            if (uncaught == nullptr) {
                uncaught = std::current_exception();
            }
        }
    }
    // probably need to fix this
    catch (...) {
        interrupted = true;
    }
    // Close waiting clients.
    for (const client_ptr& client : waiting) {
        client->close();
    }
    // Wait for clients to finish.
    // stop doesn't return until its threads have terminated.
    tp.stop();
    // Notify factory it has stopped
    if (!interrupted) {
        try {
            onStop();
        }
        catch (...) {
            if (uncaught == nullptr)
                uncaught = std::current_exception();
        }
    }
    // Empty the lists and queue.
    waiting.clear();
    running.clear();
    queue.Clear();
    //Signal that the server is done shutting down
    completion.signal();
    if (uncaught) throw uncaught;
}

void tt::ClientFactory::close()
{
    queue.Push(std::make_unique<Operation>(
        [this] {
            closed = true;
        },
        *this
    ));
}

void tt::ClientFactory::onStart()
{
    // This is meant to be overridden
}

void tt::ClientFactory::onClose()
{
    // This is meant to be overridden
}

void tt::ClientFactory::onStop()
{
    // This is meant to be overridden
}

void tt::ClientFactory::createNewClient()
{
    client_ptr client = create();
    waiting.push_back(std::move(client));

    Client* c_ptr = client.get();

    tp.enqueue(std::make_unique<Operation>(
        [this, c_ptr] {
            try {
                c_ptr->execute(this);
            }
            catch (...) {
                onStop(c_ptr);
                throw;
            }

            onStop(c_ptr);
        },
        *this
    ));
}

void tt::ClientFactory::onStart(const Client* client)
{
    tp.enqueue(std::make_unique<Operation>(
        [this, client] {
            auto it = std::find_if(waiting.begin(), waiting.end(), 
            [client](const client_ptr& ptr) {
                return ptr.get() == client;
            });

            // should never happen but just in case
            if (it == waiting.end()) {
                return;
            }
            running.push_back(std::move(*it));
            waiting.erase(it);
            if (maxClients <= 0 || waiting.size() + running.size() < maxClients) {
                createNewClient();
            }
        },
        *this
    ));
}

void tt::ClientFactory::onStop(const Client* client)
{
    tp.enqueue(std::make_unique<Operation>(
        [this, client] {
            auto it = std::find_if(running.begin(), running.end(), 
            [client](const client_ptr& ptr) {
                return ptr.get() == client;
            });
            if (it != running.end() && waiting.size() == 0) {
                running.erase(it);
                createNewClient();
            }
            else {
                // go find it in waiting
                auto it_w = std::find_if(waiting.begin(), waiting.end(), 
                [client](const client_ptr& ptr) {
                    return ptr.get() == client;
                });
                if (it_w == waiting.end()) return;
                waiting.erase(it_w);
            }
        },
        *this
    ));
}

std::ostream &tt::operator<<(std::ostream &os, const ClientFactory &a)
{
    os << a.toString();
    return os;
}
