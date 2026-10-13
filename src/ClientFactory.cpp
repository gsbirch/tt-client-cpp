#include <tt/ClientFactory.h>
#include <tt/Client.h>
#include <cxxabi.h>
#include <typeinfo>
#include <memory>

tt::ClientFactory::ClientFactory(int maxClients): 
maxClients(maxClients), tp(maxClients)
{
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
    onStart();
    bool interrupted = false;

    try {
        createNewClient();
        while (!closed) {
            std::unique_ptr<Operation> ptr;
            queue.Pop(ptr);
            if (!ptr) continue;          // avoid null deref if Pop gives nothing
            ptr->execute();
        }
        try { onClose(); }
        catch (...) { captureException("onClose"); }
    } catch (...) {
        interrupted = true;
        captureException("main loop");
    }

    // Shutdown steps must not throw, or they'd skip the remaining cleanup.
    try {
        for (const client_ptr& client : waiting) client->close();
    } catch (...) { captureException("closing clients"); }

    try { tp.stop(); }
    catch (...) { captureException("thread pool stop"); }

    if (!interrupted) {
        try { onStop(); }
        catch (...) { captureException("onStop"); }
    }

    waiting.clear();
    running.clear();
    queue.Clear();
    completion.signal();

    // Only rethrow if something actually failed.
    if (uncaught) {
        std::rethrow_exception(uncaught);
    }
}

void tt::ClientFactory::close()
{
    // multiple calls to close won't break anything
    if (closeRequested_.exchange(true)) {
        return;
    }

    queue.Push(std::make_unique<Operation>(
        [this] {
            closed = true;
        },
        *this
    ));
}

void tt::ClientFactory::captureException(const char *stage)
{
    std::exception_ptr ep = std::current_exception();   // must be called inside a catch block
    try {
        std::rethrow_exception(ep);
    } catch (const std::exception& e) {
        int status = 0;
        std::unique_ptr<char, void(*)(void*)> name(
            abi::__cxa_demangle(typeid(e).name(), nullptr, nullptr, &status), std::free);
        std::cerr << "[" << stage << "] " << (status == 0 ? name.get() : typeid(e).name())
                  << ": " << e.what() << '\n';
    } catch (...) {
        std::cerr << "[" << stage << "] non-std::exception thrown\n";
    }
    std::lock_guard<std::mutex> lock(uncaughtMutex);
    if (!uncaught) uncaught = ep;
}

void tt::ClientFactory::createNewClient()
{
    client_ptr client = create();
    Client* c_ptr = client.get();
    
    waiting.push_back(std::move(client));

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

tt::ClientFactory::~ClientFactory()
{
    close();
    completion.wait();
}

std::ostream &tt::operator<<(std::ostream &os, const ClientFactory &a)
{
    os << a.toString();
    return os;
}
