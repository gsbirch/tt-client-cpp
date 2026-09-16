#include <tt/Client.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <openssl/ssl.h>

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
    join(name, password, world, role, partner), key(key), url(url), port(port)
{
    // join(name, password, world, role, partner),
    // Utilities.requireNonNull(name, "name");
    // this.join = new Join(name, password, world, role, partner);
    // this.key = key;
    // Utilities.requireNonNull(url, "server URL");
    // this.url = url;
    // this.port = port;
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
Client(name, "ENV_TODO", world, role, partner, "ENV_TODO", url, port)
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

std::string tt::Client::start()
{
    // Warn if password or API key are missing.
    if(join.password == "" && std::getenv(ENVIRONMENT_VARIABLE_PASSWORD.c_str()) == nullptr)
        onWarning("The environment variable \"" + ENVIRONMENT_VARIABLE_PASSWORD + "\" is not set. This agent will not use a password.");
    if(key == "" && std::getenv(ENVIRONMENT_VARIABLE_API_KEY.c_str()) == nullptr)
			onWarning("The environment variable \"" + ENVIRONMENT_VARIABLE_API_KEY + "\" is not set. This agent will not be able to use the external API.");
    ssl = connect(url, port);

    running_ = true;
    receiveThread_ = std::thread(&Client::receiveLoop, this);
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


    // WILL NEED TO BE REMOVED 

        // send a fake join message
        // std::string msg = "{ \"type\": \"Join\", \"name\": \"webby\", \"password\": \"dummy\", \"world\": \"tutorial\", \"role\": \"PLAYER\", \"partner\": \"random\" }";

        // SSL_write(ssl_ptr, msg.c_str(), msg.length());
        //send(sock,msg.c_str(), msg.length(), 0);

        // char buffer[2048];
        // int n = SSL_read(ssl_ptr, buffer, sizeof(buffer) - 1);
        // buffer[n] = '\0';
        // std::cout << "Received: " << buffer << '\n';                                     

    return ssl_ptr;
}

tt::Client::~Client()
{
    stop();
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

void tt::Client::receiveLoop()
{
    char buffer[4096];

    while (running_) {
        int bytesRead = SSL_read(ssl, buffer, sizeof(buffer));

        if (bytesRead > 0) {
            processMessage(buffer, bytesRead);
        } else {
            // Handle SSL error / connection closed
            std::cerr << "There has been an SSL error, or the connection has been closed!" << std::endl;
            break;
        }
    }
}

void tt::Client::processMessage(const char *data, int length)
{
    std::string message(data, length);

    std::cout << "Received message: " << message << std::endl;
}

void tt::Client::stop()
{
    if (receiveThread_.joinable()) {
        receiveThread_.join();
    }
    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sock);
}
