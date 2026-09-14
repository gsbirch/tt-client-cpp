#include <tt/Client.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

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

std::string tt::Client::operator()(ClientFactory factory)
{
    return std::string();
}

int tt::Client::connect(std::string url, int port)
{
    // the socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    char addr[1024];
    strcpy(addr, url.c_str());

    // connect
    inet_pton(AF_INET, addr, &server.sin_addr);

    // send a fake join message
    std::string msg = "{ \"type\": \"BAD_MSG\", \"name\": \"web\", \"password\": \"dummy\", \"world\": \"tutorial\", \"role\": \"PLAYER\", \"partner\": \"random\" }";
    char c_msg[1024];
    strcpy(c_msg, msg.c_str());

    send(sock, c_msg, msg.length(), 0);

    char buffer[2048];
    int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
    buffer[n] = '\0';
    std::cout << "Received: " << buffer << '\n';

    close(sock);
}
