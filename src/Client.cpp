#include <tt/Client.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <openssl/ssl.h>
#include <tt/io/Join.h>
#include <tt/io/Start.h>
#include <tt/io/Message.h>
#include <tt/io/MessageFactory.h>


#include <cstdlib>

using namespace tt;

const std::string Client::ENVIRONMENT_VARIABLE_PASSWORD = "password";

/**
 * The name of the environment variable where the client expects to find the
 * API key used to authenticate with the service that provides functions
 * which require special external resources or computation. If this
 * environment variable is not set and no API key is provided in the
 * constructor, this agent will not be able to use the external API.
 */
const std::string Client::ENVIRONMENT_VARIABLE_API_KEY = "apikey";

/**
 * The default URL the client will attempt to connect to if one is not
 * explicitly provided in the constructor
 */
const std::string Client::DEFAULT_URL = "localhost";

/**
 * The default network port the client will attempt to connect to if one is
 * not explicitly provided in the constructor
 */
const int Client::DEFAULT_PORT = 9005;

/**
 * Constructs a client with the given session preferences, password, API
 * key, and network details.
 * 
 * @param name the name the client will use
 * @param password the password the client will provide to the server, or
 * null if the client will not use a password
 * @param world the name of the world the client wants their session to take
 * place in, or null if the client has no preference for a story world
 * @param role the role the client wants to play in the session, or null if
 * the client is willing to play either role
 * @param partner the name of the client's desired partner, or null if the
 * client is willing to play with any partner
 * @param key the API key the client will use to access external resources
 * and computation, or null if the client will not use the external API
 * @param url the URL of the server to which this client will connect
 * @param port the network port on which this client will connect
 */
Client::Client(std::string name, std::string password, std::string world, Role role, std::string partner, std::string key, std::string url, int port):
key(key), url(url), port(port)
{
    // should do these?? idk how thats gonna work with the constructors being before
    // Utilities.requireNonNull(name, "name");
    // Utilities.requireNonNull(url, "server URL");
    join = std::make_unique<Join>(name, password, world, role, partner);

    registerMessageTypes();
}

/**
 * Constructs a client with the given session preferences and network
 * details, reading the password and API key from the environment. The
 * client's password will be read from {@link
 * #ENVIRONMENT_VARIABLE_PASSWORD}. The client's API key will be read from
 * {@link #ENVIRONMENT_VARIABLE_API_KEY}.
 * 
 * @param name the name the client will use
 * @param world the name of the world the client wants their session to take
 * place in, or null if the client has no preference for a story world
 * @param role the role the client wants to play in the session, or null if
 * the client is willing to play either role
 * @param partner the name of the client's desired partner, or null if the
 * client is willing to play with any partner
 * @param url the URL of the server to which this client will connect
 * @param port the network port on which this client will connect
 */
Client::Client(std::string name, std::string world, Role role, std::string partner, std::string url, int port):
Client(name, std::getenv(ENVIRONMENT_VARIABLE_PASSWORD.c_str()), world, role, partner, std::getenv(ENVIRONMENT_VARIABLE_API_KEY.c_str()), url, port)
{

}

/**
 * Constructs a client with the given session preferences, reading the
 * password and API key from the environment, and using the default network
 * settings.
 * 
 * @param name the name the client will use
 * @param world the name of the world the client wants their session to take
 * place in, or null if the client has no preference for a story world
 * @param role the role the client wants to play in the session, or null if
 * the client is willing to play either role
 * @param partner the name of the client's desired partner, or null if the
 * client is willing to play with any partner
 */
Client::Client(std::string name, std::string world, Role role, std::string partner):
Client(name, world, role, partner, DEFAULT_URL, DEFAULT_PORT) 
{

}

/**
 * Constructs a client with a name, role, and world name, which has no
 * preference for a partner, reading the password and API key from the
 * environment, and using the default network settings.
 * 
 * @param name the name the client will use
 * @param world the name of the world the client wants their session to take
 * place in, or null if the client has no preference for a story world
 * @param role the role the client wants to play in the session, or null if
 * the client is willing to play either role
 */
Client::Client(std::string name, std::string world, Role role):
Client(name, world, role, "") 
{

}

/**
 * Constructs a client with a given name, which has no preference for its
 * role, world, or partner, reading the password and API key from the
 * environment, and using the default network settings.
 * 
 * @param name the name the client will use
 */
Client::Client(std::string name):
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

std::string tt::Client::getName() const
{
    return join->name;
}

std::string tt::Client::getWorldName() const
{
    // if(world == null)
	// 		return join->world;
	// 	else
	// 		return world.name;
    return "";
}

Role tt::Client::getRole() const
{
    if(role == Role::NONE)
        return join->role;
    else
        return role;
}

std::string tt::Client::getPartner() const
{
    return join->partner;
}

std::string tt::Client::execute()
{
    return execute(nullptr);
}

std::string tt::Client::start()
{
    // Warn if password or API key are missing.
    if(join->password == "" && std::getenv(ENVIRONMENT_VARIABLE_PASSWORD.c_str()) == nullptr)
        onWarning("The environment variable \"" + ENVIRONMENT_VARIABLE_PASSWORD + "\" is not set. This agent will not use a password.");
    if(key == "" && std::getenv(ENVIRONMENT_VARIABLE_API_KEY.c_str()) == nullptr)
		onWarning("The environment variable \"" + ENVIRONMENT_VARIABLE_API_KEY + "\" is not set. This agent will not be able to use the external API.");
    ssl = connect(url, port);

    running_ = true;
    receiveThread_ = std::thread(&Client::receiveLoop, this);

    std::unique_ptr<Connect> connect = std::move(receive<Connect>());
    std::cout << *connect << std::endl;

    sendMessage(*join);

    std::unique_ptr<Start> start = std::move(receive<Start>());
    std::cout << *start << std::endl;

    return "";
}

std::string tt::Client::execute(ClientFactory* factory)
{
    start();
    return std::string();
}

SSL* tt::Client::connect(std::string url, int port)
{
    // the socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // connect
    inet_pton(AF_INET, url.c_str(), &server.sin_addr);

    ::connect(sock, (sockaddr*)&server, sizeof(server));

    // SSL Setup
    ctx = SSL_CTX_new(TLS_client_method());
    SSL* ssl_ptr = SSL_new(ctx);
    SSL_set_fd(ssl_ptr, sock);

    // Start TLS
    SSL_connect(ssl_ptr);
    
    return ssl_ptr;
}

tt::Client::~Client()
{
    close();
}

void tt::Client::sendMessage(const tt::Message& m)
{
    json j;
    j["type"] = m.type();

    if (const auto* join = dynamic_cast<const tt::Join*>(&m)) {
        j = *join;
    }

    std::string s = j.dump() + "\n";
    sendMessage(s);
}

void tt::Client::sendMessage(const std::string &s)
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

void tt::Client::onWarning(std::string message)
{
    std::cerr << "Warning: " << message << std::endl;
}

void tt::Client::onError(std::string message)
{
    // if(world == "")
    //     throw std::runtime_error(message);
    // else
    //     System.err.println("Error: " + message);
}

std::string tt::Client::complete(std::string system, std::string prompt, float temperature)
{
    getKey();
    // TODO
    return "";
}

int tt::Client::embed(std::string string, float f[])
{
    getKey();
    // TODO
    return -1;
}


void tt::Client::setName(std::string name)
{
    failIfJoined("name");
	join = std::make_unique<Join>(name, join->password, join->world, join->role, join->partner);
}

void tt::Client::setPassword(std::string password)
{
    failIfJoined("password");
	join = std::make_unique<Join>(join->name, password, join->world, join->role, join->partner);
}

void tt::Client::setWorldName(std::string world)
{
    failIfJoined("world");
	// join = new Join(join.name, join.password, world, join.role, join.partner);
}

void tt::Client::setRole(Role role)
{
    failIfJoined("role");
	join = std::make_unique<Join>(join->name, join->password, join->world, role, join->partner);
}

void tt::Client::setPartner(std::string partner)
{
    failIfJoined("partner");
	join = std::make_unique<Join>(join->name, join->password, join->world, join->role, partner);
}

std::string tt::Client::getSession()
{
    return session;
}

void tt::Client::onStop(std::string message)
{
    // This method is meant to be overridden.
}

void tt::Client::onClose()
{
    // This method is meant to be overridden.
}

void tt::Client::onDisconnect()
{
    // This method is meant to be overridden.
}

void tt::Client::failIfJoined(std::string property)
{
    if(joined)
		throw std::logic_error("The client's " + property + " can no longer be changed because the client has already joined the server.");
}

std::string tt::Client::getKey() const
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
            auto msg = processMessage(buffer, bytesRead);
            if (msg) {
                messageQueue_.Push(std::move(msg));
            }
        } else {
            // Handle SSL error / connection closed
            std::cerr << "There has been an SSL error, or the connection has been closed!" << std::endl;
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

std::unique_ptr<Message> tt::Client::receiveAny() {
    
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
inline T Client::failIfNotStarted(T object, std::string description)
{
    return object;
    // if(object == null)
    //     throw std::logic_error("The " + description + " is not available because the client's session has not started yet.");
    // else
    //     return object;
}