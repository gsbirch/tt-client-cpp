#include <string>
#include <tt/Role.h>
#include <tt/io/Join.h>

#pragma once

#ifndef CLIENT_H
#define CLIENT_H

namespace tt {
    class Client {
        public:
            /**
             * The name of the environment variable where the client expects to find the
             * password this agent will use, unless the password is explicitly set by
             * the agent's constructor. If this environment variable is not set and no
             * password is provided in the constructor, this agent will not use a
             * password.
             */
            const static std::string ENVIRONMENT_VARIABLE_PASSWORD;

            /**
             * The name of the environment variable where the client expects to find the
             * API key used to authenticate with the service that provides functions
             * which require special external resources or computation. If this
             * environment variable is not set and no API key is provided in the
             * constructor, this agent will not be able to use the external API.
             */
            const static std::string ENVIRONMENT_VARIABLE_API_KEY;

            /**
             * The default URL the client will attempt to connect to if one is not
             * explicitly provided in the constructor
             */
            const static std::string DEFAULT_URL;

            /**
             * The default network port the client will attempt to connect to if one is
             * not explicitly provided in the constructor
             */
            const static int DEFAULT_PORT;

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
            Client(std::string name, std::string password, std::string world, Role role, std::string partner, std::string key, std::string url, int port);

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
            Client(std::string name, std::string world, Role role, std::string partner, std::string url, int port);

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
            Client(std::string name, std::string world, Role role, std::string partner);
            
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
            Client(std::string name, std::string world, Role role);
            
            /**
             * Constructs a client with a given name, which has no preference for its
             * role, world, or partner, reading the password and API key from the
             * environment, and using the default network settings.
             * 
             * @param name the name the client will use
             */
            Client(std::string name);

        private:
            /** The client's API key */
            std::string key;

            /** The URL where the client will attempt to connect */
            std::string url;

            /** The network port the client will attempt to connect on */
            int port;

            /**
             * The client's join request, containing their name, password, and other
             * session preferences
             */
            Join join;

            /** The secure socket used to connect to the server */
            // private SSLSocket socket = null;
            
            // /** Used to read messages received over the socket */
            // private ClientInput input = null;
            
            // /** Used to send messages over the socket */
            // private ClientOutput output = null;
            
            /** Whether the client has sent its join request yet */
            bool joined = false;
            
            // /** The world in which this client's story takes places */
            // private World world = null;
            
            /** This client's role in the story */
            Role role = Role::NONE;
            
            // /** The current status of the story world as received from the server */
            // private Status status = null;
            
            // /** The current choices available to the client */
            // private List<Turn> choices = null;
            
            // /** The stop message received from the server */
            // private Stop stop = null;
            
            // /** The session ID received from the server at the end of the session */
            // private String session = null;
    };
}


#endif