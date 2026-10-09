#include <tt/Client.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <openssl/ssl.h>
#include <netdb.h>
#include <stdexcept>
#include <tt/io/Join.h>
#include <tt/io/Start.h>
#include <tt/io/Message.h>
#include <tt/io/MessageFactory.h>
#include <tt/Settings.h>
#include <tt/ClientFactory.h>
#include <tt/io/Update.h>
#include <tt/io/End.h>
#include <tt/io/Error.h>
#include <tt/io/Choice.h>
#include <tt/util/Util.h>
#include <cstdlib>

using namespace tt;

// Setting static strings
const std::string Client::ENVIRONMENT_VARIABLE_PASSWORD = "password";
const std::string Client::ENVIRONMENT_VARIABLE_API_KEY = "apikey";
const std::string Client::DEFAULT_URL = "localhost";
const int Client::DEFAULT_PORT = 9005;

Client::Client(const std::string& name, const std::string& password, const std::string& world, Role role, const std::string& partner, const std::string& key, const std::string& url, int port):
key(key), url(url), port(port)
{
    requireNonEmpty(name, "name");
    requireNonEmpty(url, "server URL");
    join = std::make_unique<Join>(name, password, world, role, partner);

    registerMessageTypes();
}

Client::Client(const std::string& name, const std::string& world, Role role, const std::string& partner, const std::string& url, int port):
Client(name, std::getenv(ENVIRONMENT_VARIABLE_PASSWORD.c_str()), world, role, partner, std::getenv(ENVIRONMENT_VARIABLE_API_KEY.c_str()), url, port)
{
}

Client::Client(const std::string& name, const std::string& world, Role role, const std::string& partner):
Client(name, world, role, partner, DEFAULT_URL, DEFAULT_PORT) 
{

}

Client::Client(const std::string& name, const std::string& world, Role role):
Client(name, world, role, "") 
{

}

Client::Client(const std::string& name):
Client(name, "", Role::NONE, "")
{
    
}

std::string tt::Client::toString() const
{
    std::string string = "[Client: name=\"" + join->name + "\"";
    if(join->password != "")
        string += "; password=\"***\"";
    if(join->world != "")
        string += "; world=\"" + join->world + "\"";
    if(join->role != Role::NONE)
        string += "; role=" + join->role;
    if(join->partner != "")
        string += "; partner=\"" + join->partner + "\"";
    return string + "]";
}

const std::string& tt::Client::getName() const
{
    return join->name;
}

const std::string& tt::Client::getWorldName() const
{
    if(world == nullptr)
        return join->world;
    else
        return world->name;
}

Role tt::Client::getRole() const
{
    if(role == Role::NONE)
        return join->role;
    else
        return role;
}

const std::string& tt::Client::getPartner() const
{
    return join->partner;
}

const World* tt::Client::getWorld()
{
    return failIfNotStarted(world, "world");
}

std::vector<const Turn *> tt::Client::getHistory() const
{
    return failIfNotStarted(status, "history")->getHistory();
}

const State *tt::Client::getState() const
{
    return failIfNotStarted(status, "state")->getState();
}

std::string tt::Client::execute()
{
    return execute(nullptr);
}


std::string tt::Client::execute(ClientFactory* factory)
{
    // Warn if password or API key are missing.
    if(join->password == "" && std::getenv(ENVIRONMENT_VARIABLE_PASSWORD.c_str()) == nullptr)
        onWarning("The environment variable \"" + ENVIRONMENT_VARIABLE_PASSWORD + "\" is not set. This agent will not use a password.");
    if(key == "" && std::getenv(ENVIRONMENT_VARIABLE_API_KEY.c_str()) == nullptr)
		onWarning("The environment variable \"" + ENVIRONMENT_VARIABLE_API_KEY + "\" is not set. This agent will not be able to use the external API.");
    
    // Connect the socket
    ssl = connect(url, port);

    // Run a thread for receiving messages
    running_ = true;
    receiveThread_ = std::thread(&Client::receiveLoop, this);

    // If an exception is thrown, catch it to throw later
    std::exception_ptr uncaught = nullptr;
    try {
        // Wait for the connect message
        std::unique_ptr<Connect> connect = std::move(receive<Connect>());
        if (connect != nullptr) {
            // Warn if the server's version number does not match.
            // if(connect->version != Settings::VERSION)
                // onWarning("This client is using version " + Settings::VERSION + " of the communication protocol, but the server is using version " + connect->version + ". This may cause misconnunications.");
            // Notify the client is has connected.
			onConnect(connect.get());
            // Send the join message.
            joined = true;
            sendMessage(*join);
        }
        // Wait for the start message.
        std::unique_ptr<Start> start = std::move(receive<Start>());
        if (start != nullptr) {
            world = std::move(start->world);
            role = start->role;
            // Notify the factory that created this client that its session has started.
            if (factory != nullptr)
                factory->onStart(this);
        }
        // Receive and process messages;
        while (world != nullptr) {
            std::unique_ptr<Message> message = receiveAny();
            // Stop if disconnected
            if (message == nullptr)
                break;
            // When the world status updates...
            if (Update* updatePtr = dynamic_cast<Update*>(message.get())) {
                message.release();
                std::unique_ptr<Update> update(updatePtr);
                if (status == nullptr) {
                    status = std::move(update->status);
                    choices = status->getChoices();
                    onStart(world.get(), role, status->getState());
                }
                else {
                    status = std::move(update->status);
                    choices = status->getChoices();
                    onUpdate(status.get());
                }
                // If it is the client's turn, make a choice.
                if (status->getChoices().size() > 0) {
                    int index = onChoice(status.get());
                    choices.clear();
                    Choice c(index);
                    sendMessage(c);
                }
                // If the story has ended, notify the client.
                else if (status->getEnding() != nullptr) {
                    onEnd(status->getEnding());
                }
            }
            // Immediately acknowledge stop messages.
            else if (Stop* stopPtr = dynamic_cast<Stop*>(message.get())) {
                message.release();
                std::unique_ptr<Stop> stop(stopPtr);
                Stop s(role);
                sendMessage(s);
                onStop(stop->message);
            }
            // Get session ID from end message.
            else if (End* endPtr = dynamic_cast<End*>(message.get())) {
                message.release();
                std::unique_ptr<End> end(endPtr);
                session = end->session;
                break;
            }
            else if (Error* errorPtr = dynamic_cast<Error*>(message.get())) {
                message.release();
                std::unique_ptr<Error> error(errorPtr);
                onError(error->message);
            }
            else
                onError("The message type \"" +  message->type() + "\" is not recognized.");
        }
    }
    catch (...) {
        uncaught = std::current_exception();
    }
    // If the session has started but not yet stopped, notify.
    if (uncaught == nullptr) {
        try {
            onClose();
        }
        catch (...) {
            uncaught = std::current_exception();
        }
    }
    // Ensure the socket is closed.
    close();
    // Notify the client it has disconnected.
    try {
        onDisconnect();
    }
    catch (...) {
        if (uncaught == nullptr)
            uncaught = std::current_exception();
    }
    // Throw an uncaught exception or return the session ID.
    if(uncaught == nullptr)
        return session;
    else
        throw uncaught;
}

SSL* tt::Client::connect(const std::string& url, int port)
{
    if (port < 1 || port > 65535)
        throw std::invalid_argument("Port out of range: " + std::to_string(port));

    // Resolve the host name
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    addrinfo* result = nullptr;
    int rc = getaddrinfo(url.c_str(), std::to_string(port).c_str(), &hints, &result);
    if (rc != 0)
        throw std::invalid_argument("Unknown host '" + url + "': " + gai_strerror(rc));

    // Connect
    sock = ::socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sock < 0) {
        freeaddrinfo(result);
        throw std::runtime_error("Failed to create socket");
    }

    if (::connect(sock, result->ai_addr, result->ai_addrlen) < 0) {
        freeaddrinfo(result);
        throw std::runtime_error("Failed to connect to " + url);
    }
    freeaddrinfo(result);

    // SSL setup
    ctx = SSL_CTX_new(TLS_client_method());
    SSL* ssl_ptr = SSL_new(ctx);
    SSL_set_fd(ssl_ptr, sock);

    // Start TLS
    if (SSL_connect(ssl_ptr) != 1)
        throw std::runtime_error("TLS handshake failed");

    return ssl_ptr;
}

tt::Client::~Client()
{
    close();
}

void tt::Client::sendMessage(const tt::Message& m)
{
    if (ssl == nullptr) 
        throw std::logic_error("A message cannot be sent because the client has not connected to the server yet.");
    json j;
    j["type"] = m.type();

    if (const auto* join = dynamic_cast<const tt::Join*>(&m)) {
        j = *join;
    }

    std::string s = j.dump() + "\n";
    sendMessage(s);
}

void tt::Client::sendMessage(const std::string& s)
{
    const char * data = s.data();
    std::size_t remaining = s.size();

    while (remaining > 0) {
        int written = SSL_write(
            ssl,
            data,
            static_cast<int>(remaining)
        );
        

        if (written <= 0) {
            int error = SSL_get_error(ssl, written);
            throw std::runtime_error(
                "SSL_write failed: " + std::to_string(error)
            );
        }

        
        data += written;
        remaining -= written;
    }
}

void tt::Client::onWarning(const std::string& message)
{
    std::cerr << "Warning: " << message << std::endl;
}

void tt::Client::onError(const std::string& message)
{
    if(world == nullptr)
        throw std::runtime_error(message);
    else
        std::cerr << "Error: " << message << std::endl;
}

std::string tt::Client::complete(const std::vector<std::string> &messages, const std::string &reasoning, int maxt, float temp, int topk, float topp, float minp)
{
    getKey();
    return std::string();
}


void tt::Client::setName(const std::string& name)
{
    failIfJoined("name");
	join = std::make_unique<Join>(name, join->password, join->world, join->role, join->partner);
}

void tt::Client::setPassword(const std::string& password)
{
    failIfJoined("password");
	join = std::make_unique<Join>(join->name, password, join->world, join->role, join->partner);
}

void tt::Client::setWorldName(const std::string& world)
{
    failIfJoined("world");
	join = std::make_unique<Join>(join->name, join->password, world, join->role, join->partner);
}

void tt::Client::setRole(Role role)
{
    failIfJoined("role");
	join = std::make_unique<Join>(join->name, join->password, join->world, role, join->partner);
}

void tt::Client::setPartner(const std::string& partner)
{
    failIfJoined("partner");
	join = std::make_unique<Join>(join->name, join->password, join->world, join->role, partner);
}

std::vector<const Turn *> tt::Client::getChoices() const
{
    // return failIfNotStarted(choices, "list of choices");
    return choices;
}

const std::string &tt::Client::getSession()
{
    return session;
}

void tt::Client::failIfJoined(const std::string& property)
{
    if(joined)
		throw std::logic_error("The client's " + property + " can no longer be changed because the client has already joined the server.");
}

const std::string& tt::Client::getKey() const
{
    if(key == "")
        throw std::logic_error("The client does not have an API key, so it cannot use the external API.");
    else
        return key;
}

void tt::Client::receiveLoop()
{
    char buffer[4096];

    while (running_) {
        int bytesRead = SSL_read(ssl, buffer, sizeof(buffer));

        if (bytesRead > 0) {
            recvBuffer_.append(buffer, bytesRead);

            // Extract as many complete (newline-terminated) messages as are available
            size_t pos;
            while ((pos = recvBuffer_.find('\n')) != std::string::npos) {
                std::string line = recvBuffer_.substr(0, pos);
                recvBuffer_.erase(0, pos + 1);

                if (line.empty()) continue; // skip stray blank lines

                auto msg = processMessage(line.data(), line.size());
                if (msg) {
                    messageQueue_.Push(std::move(msg));
                }
            }
        } else {
            std::cerr << "There has been an SSL error, or the connection has been closed!" << std::endl;
            running_ = false;
            break;
        }
    }
}

std::unique_ptr<Message> tt::Client::processMessage(const char *data, int length)
{
    std::string message(data, length);

     nlohmann::json j;
    try {
        j = nlohmann::json::parse(data, data + length);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Failed to parse message JSON: " << e.what() << std::endl;
        return nullptr;
    }

    std::string type = j["type"].get<std::string>();

    auto msg = MessageFactory::instance().create(type, j);
    if (!msg) {
        std::cerr << "Unknown message type: " << type << std::endl;
    }
    return msg;
}

void tt::Client::close()
{
    // This mainly cleans up SSL stuff, so if it isn't open yet, do nothing
    if (ssl == nullptr) return;

    if (receiveThread_.joinable()) {
        receiveThread_.join();
    }
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    ::close(sock);
}

std::ostream &tt::operator<<(std::ostream &os, const Client &a)
{
    os << a.toString();
    return os;
}

std::vector<float> tt::Client::embed(std::string text, int dim)
{
    std::string key = getKey();
    return std::vector<float>();
}

std::unique_ptr<Message> tt::Client::receiveAny()
{

    std::unique_ptr<Message> msg;
    if (!messageQueue_.Pop(msg)) {
        throw std::runtime_error("Connection closed while waiting for message");
    }
    return msg;
}

template <typename T>
std::unique_ptr<T> tt::Client::receive()
{
    std::unique_ptr<Message> msg = receiveAny();

    if (T* typed = dynamic_cast<T*>(msg.get())) {
        msg.release();
        return std::unique_ptr<T>(typed);
    }

    throw std::runtime_error(
        "Expected message type '" + std::string(typeid(T).name()) +
        "' but received type '" + msg->type() + "'");
}

template <typename T>
const T *tt::Client::failIfNotStarted(const T *object, const std::string &description)
{
    if(object == nullptr)
        throw std::logic_error("The " + description + " is not available because the client's session has not started yet.");
    else
        return object;
}

template <typename T>
const T *tt::Client::failIfNotStarted(const std::unique_ptr<T> &object, const std::string &description)
{
    return failIfNotStarted(object.get(), description);
}
