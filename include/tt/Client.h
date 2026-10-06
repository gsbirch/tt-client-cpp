#pragma once

#include <string>
#include <tt/Role.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <thread>
#include <tt/world/World.h>
#include <tt/io/Stop.h>
#include <tt/world/Status.h>
#include <tt/world/Turn.h>
#include <tt/io/Connect.h>
#include <tt/world/State.h>
#include <tt/Role.h>
#include <tt/util/BlockingQueue.h>
#include <atomic>

#ifndef CLIENT_H
#define CLIENT_H

namespace tt {
    // forward declarations to avoid circular inheritance
    class ClientFactory;
    class Join;
    class Message;
    /** 
     * A client connects to a server to find a partner and play a
     * {@link tt::Role role} in a storytelling session.
     * <p>
     * This abstract class implements the necessary communication protocol to
     * connect to the server, find a partner, and start a session. Each time a
     * {@link Status status update} arrives from the server that requires this
     * client to make a choice, the {@link #onChoice} method is called to
     * determine what choice the agent wants to make.
     * <p>
     * This class provides several other methods that are all called from the same
     * thread at important moments in the client's lifecycle. These methods can be
     * overridden to, for example, log important information or update the client's
     * world model. See the {@link #execute()} method for a full description of these
     * methods and when they are called.
     * <p>
     * If the session begins and ends normally the client's {@link #execute()} method
     * returns the session ID of the completed session. If this client disconnects
     * early, or if a problem occurs that does not cause an uncaught exception, that
     * method will return null.
     * 
     * @author Gage Birchmeier
     */
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
             * empty if the client will not use a password
             * @param world the name of the world the client wants their session to take
             * place in, or empty if the client has no preference for a story world
             * @param role the role the client wants to play in the session, or empty if
             * the client is willing to play either role
             * @param partner the name of the client's desired partner, or empty if the
             * client is willing to play with any partner
             * @param key the API key the client will use to access external resources
             * and computation, or empty if the client will not use the external API
             * @param url the URL of the server to which this client will connect
             * @param port the network port on which this client will connect
             */
            Client(const std::string& name, const std::string& password, const std::string& world, Role role, const std::string& partner, const std::string& key, const std::string& url, int port);

            /**
             * Constructs a client with the given session preferences and network
             * details, reading the password and API key from the environment. The
             * client's password will be read from {@link #ENVIRONMENT_VARIABLE_PASSWORD}. 
             * The client's API key will be read from {@link #ENVIRONMENT_VARIABLE_API_KEY}.
             * 
             * @param name the name the client will use
             * @param world the name of the world the client wants their session to take
             * place in, or empty if the client has no preference for a story world
             * @param role the role the client wants to play in the session, or empty if
             * the client is willing to play either role
             * @param partner the name of the client's desired partner, or empty if the
             * client is willing to play with any partner
             * @param url the URL of the server to which this client will connect
             * @param port the network port on which this client will connect
             */
            Client(const std::string& name, const std::string& world, Role role, const std::string& partner, const std::string& url, int port);

            /**
             * Constructs a client with the given session preferences, reading the
             * password and API key from the environment, and using the default network
             * settings.
             * 
             * @param name the name the client will use
             * @param world the name of the world the client wants their session to take
             * place in, or empty if the client has no preference for a story world
             * @param role the role the client wants to play in the session, or empty if
             * the client is willing to play either role
             * @param partner the name of the client's desired partner, or empty if the
             * client is willing to play with any partner
             */
            Client(const std::string& name, const std::string& world, Role role, const std::string& partner);
            
            /**
             * Constructs a client with a name, role, and world name, which has no
             * preference for a partner, reading the password and API key from the
             * environment, and using the default network settings.
             * 
             * @param name the name the client will use
             * @param world the name of the world the client wants their session to take
             * place in, or empty if the client has no preference for a story world
             * @param role the role the client wants to play in the session, or empty if
             * the client is willing to play either role
             */
            Client(const std::string& name, const std::string& world, Role role);
            
            /**
             * Constructs a client with a given name, which has no preference for its
             * role, world, or partner, reading the password and API key from the
             * environment, and using the default network settings.
             * 
             * @param name the name the client will use
             */
            Client(const std::string& name);

            /**
             * Gives a string representation of this client.
             * 
             * @returns a string representation of this client.
             */
            virtual std::string toString() const;

            /**
             * Returns the client's name.
             * 
             * @return the client's name
             */
            const std::string& getName() const;

            /**
             * Returns the name of the story world this client will play in or is
             * currently playing in. An empty value means this client's session has not
             * yet started this client is willing to play in any world. Once the session
             * starts, this method will return the name of the world the client is
             * playing in.
             * 
             * @return the name of the client's story world
             */
            const std::string& getWorldName() const;

            /**
             * Returns the role this client wants to play or is currently playing in its
             * session. The NONE value means this client's session has not yet started and
             * this client is willing to play either role. Once the session starts, this
             * method will return the role assigned to the client.
             * 
             * @return the client's role
             */
            Role getRole() const;

            /**
             * Returns the name of the partner this client wants to play with. An empty
             * value means this client is willing to play with any partner. This value
             * does not change when the session starts. In other words, if the client
             * did not request a specific partner, they client has no way to know the
             * name of the partner they were assigned.
             * 
             * @return the name of the partner this client wants to play with
             */
            const std::string& getPartner() const;

             /**
             * Returns a const pointer to the {@link World story world} in which this
             * client's session is taking place.
             * 
             * @return the story world for this client's session
             * @throws std::logic_error if the client's session has not yet started
             */
            const World* getWorld();

            /**
             * Returns the list of all {@link Turn turns} that have been taken so far in
             * this client's session that this client has observed. If the client is the
             * {@link tt::Role#GAME_MASTER game master}, they always observe all turns. If
             * this client is the {@link tt::Role#PLAYER player}, they may not observe all
             * turns.
             * 
             * @return the list of all turns that have been taken so far in the session
             * @throws std::logic_error if the client's session has not yet started
             */
            std::vector<const Turn *> getHistory() const;
            
            /**
             * Returns the current {@link State state} of the story world.
             * 
             * @return the current state of the story world
             * @throws std::logic_error if the client's session has not yet started
             */
            const State * getState() const;
            
            /**
             * If it is currently this client's turn to act, this method returns the
             * list of {@link Turn turns} they can take next. If it is not the client's
             * turn, the list will be empty.
             * 
             * @return the list of turns the client can take next
             * @throws std::logic_error if the client's session has not yet started
             */
            std::vector<const Turn *> getChoices() const;

            /**
             * Returns the ID of this client's session. A session is only assigned after
             * the session has ended, so the value returned by this method will always
             * be empty until the very end of this client's lifecycle.
             * 
             * @return the session ID of this client's session
             */
            const std::string& getSession();

            /**
             * <p>
             * This method connects to the server, joins, waits for a session to start,
             * sends the choices made by this client, and eventually returns the session
             * ID of the completed session.
             * <p>
             * As this method runs, it calls other methods to notify the client of
             * important events. Some of these methods are guaranteed to be called even
             * if an exception is thrown. The list of those methods and when they happen
             * is as follows:
             * <ul>
             * <li>The {@link #connect} method is called to establish a
             * secure socket to the server. If an exception is thrown by this method,
             * it will be thrown immediately, and the rest of the methods below will not
             * be called.</li>
             * <li>After the client receives the {@link Connect connect} message from
             * the server, {@link #onConnect} is called. This is the client's
             * last chance to make changes to its identity or session details.</li>
             * <li>After the client receives the {@link Start start} message and the
             * first {@link Update update} message, the session begins and the
             * {@link #onStart} method is called. Immediately before that
             * method is called, methods like {@link #getWorld} will be able to return
             * their values.</li>
             * <li>Each time the client receives an {@link Update update} message, the
             * {@link #onUpdate} method is called to notify the client about
             * changes to the session history and world state. This method is called
             * whether or not the client needs to make a choice.</li>
             * <li>Each time the client receives an {@link Update update} message and it
             * is the client's turn to act, after calling {@link #onUpdate} the
             * {@link #onChoice} method will be called to solicit which turn the
             * client wants to take. After calling that method, the {@link #getChoices} 
             * method will return an empty list.</li>
             * <li>If the session's story reaches one of its pre-defined endings, the
             * {@link #onEnd} method will be called.</li>
             * <li>If the session ended because this client was {@link #close closed},
             * because the client thread was interrupted, or because the socket was
             * disconnected, the {@link #onClose} method will be called. If an
             * exception was thrown earlier in this method, {@link #onClose} will not
             * be called.</li>
             * <li>If the session started, the {@link #onStop} method will
             * always be called before this method returns or throw an exception.</li>
             * <li>If the client successfully connected to the server, the 
             * {@link #onDisconnect} method will always be called before this method 
             * returns or throws an exception.</li>
             * </ul>
             */
            std::string execute();
            std::string execute(ClientFactory* factory);

            /**
             * Establishes a secure socket to the server based on this client's network
             * configuration.
             * <p>
             * By default, this method resolves the host name, opens a TCP connection,
             * performs a TLS handshake using OpenSSL's TLS client method, and returns a
             * new SSL session.
             * <p>
             * The caller takes ownership of the returned object and is responsible for
             * releasing it with <code>SSL_free()</code>.
             *
             * @param url the host name or IP address of the server
             * @param port the network port on which to open the socket
             * @return a secure connection to the server
             * @throws std::runtime_error if a problem occurred establishing the socket,
             * connecting to the server, or completing the TLS handshake
             * @throws std::invalid_argument if the URL of the server cannot be resolved
             * @throws std::invalid_argument if the network port is outside the specified
             * range of valid port values
             */
            SSL* connect(const std::string& url, int port);

            ~Client();

            /**
             * This method sends a message to the server.
             * <p>
             * It is usually unsafe for implementations of this abstract class to call
             * this method. The {@link #execute()} method has specific expectations about
             * when and what type of messages should be sent. Calling this method at an
             * unexpected time is likely to cause this client to get out of sync with
             * the server.
             * 
             * @param m the message to send to the server
             * @throws std::logic_error if the client has not yet connected to the server
             */
            void sendMessage(const tt::Message& m);
            

            /**
             * <p>
             * Disconnects this client from the server, causing it to stop. This method
             * can be called safely from any thread. If this method is called before the
             * client connects (e.g. before {@link #execute()}), it does nothing.
             */
            void close();

            friend std::ostream& operator<<(std::ostream& os, const Client& a);

        protected:
            /**
             * Sets the name this client will use.
             * 
             * @param name the new name this client will use
             * @throws std::logic_error if the client has already joined the server
             */
            void setName(const std::string& name);

            /**
             * Sets the password this client will use.
             * 
             * @param password the new password this client will use
             * @throws std::logic_error if the client has already joined the server
             */
            void setPassword(const std::string& password);

            /**
             * Sets the name of the story world this client wants to play in.
             * 
             * @param world the name of the world this client wants to play in
             * @throws std::logic_error if the client has already joined the server
             */
            void setWorldName(const std::string& world);

            /**
             * Sets the role this client wants to play in its session.
             * 
             * @param role the role this client wants to play in its session
             * @throws std::logic_error if the client has already joined the server
             */
            void setRole(Role role);

            /**
             * Sets the name of the partner this client wants to play in its session.
             * 
             * @param partner the name of the partner this client wants to play with
             * @throws std::logic_error if the client has already joined the server
             */
            void setPartner(const std::string& partner);

            /**
             * This method is called after the client connects to the server and the
             * server sends the list of available worlds and agents. This method is
             * typically the last chance the client has to change its identity, such as
             * its {@link #setName name} or {@link #setRole requested role}, before the 
             * client's join request is sent.
             * <p>
             * By default, this method does nothing. It can be overridden if the client
             * wants to react to connecting to the server.
             * 
             * @param connect the connect message sent from the server
             */
            virtual void onConnect(const Connect* connect){
                // This method is meant to be overridden.
            };
            
            /**
             * This method is called once when the client's session starts.
             * <p>
             * By default, this method does nothing. It can be overridden if the client
             * wants to react to its session starting.
             * 
             * @param world the story world in which the session will take place
             * @param role the role this client will play in the story
             * @param initial the initial state of the story world before the story
             * begins
             */
            virtual void onStart(const World* world, Role role, const State* initial) {
                // This method is meant to be overridden.
            }
            
            /**
             * This method is called each time the story world changes as a result of a
             * turn the client observes. If this client is the {@link tt::Role#GAME_MASTER
             * game master}, it will observe all turns. If this client is the
             * {@link tt::Role#PLAYER player}, it may not observe all turns.
             * <p>
             * By default, this method does nothing. It can be overridden if the client
             * wants to react to a change in the world state.
             * 
             * @param status the current status of the story world, including the
             * history of all turns and the current world state
             */
            virtual void onUpdate(const Status* status) {
                // This method is meant to be overridden.
            }
            
            /**
             * This method is called each time it is this client's turn to make a choice
             * in the story. Calls to this method will always be preceded by a call to
             * {@link #onStart} if this is the start of the session
             * or {@link #onUpdate} if this is not the start of the session.
             * 
             * @param status the current status of the story world, including the 
             * {@link tt::Status#getHistory history of all turns so far}, the 
             * {@link tt::Status#getState current state} of the world, and the 
             * {@link tt::Status#getChoices list of choices} available for the client to choose
             * from
             * @return the index (starting at 0) of the turn this client wants to take
             * from the {@link tt::Status#getChoices list of choices} given
             */
            virtual int onChoice(const Status* status) = 0;
            
            /**
             * This method is called once if the story reaches one of its 
             * {@link tt::World#getEndings pre-defined endings}. If the session never 
             * starts or if the story does not reach an ending, this method will not be called.
             * <p>
             * By default, this method does nothing. It can be overridden if the client
             * wants to react to the end of the story.
             * 
             * @param ending the ending of the story
             */
            virtual void onEnd(const Ending *ending) {
                // This method is meant to be overridden.
            }

            /**
             * This method is called once when the client's session ends. If the session
             * started, this method will always be called, even if an uncaught exception
             * was thrown at an earlier stage in the client's lifecycle.
             * <p>
             * By default, this method does nothing. It can be overridden if the client
             * wants to react to the end of the session.
             * 
             * @param message the message explaining why the session ended, or null if
             * no explanation was received
             */
            virtual void onStop(const std::string& message){
                // This method is meant to be overridden.
            };

            /**
             * This method is called once if the client was {@link #close closed}, if
             * it was interrupted, or if the socket was disconnected. This method will
             * not be called if the client is stopping because an uncaught exception was
             * thrown at an earlier stage of the client's lifecycle. This method is
             * always called from the {@link #execute()} method, meaning it will always run
             * on the thread which called that method, even if a different thread called
             * the {@link #close} method.
             * <p>
             * By default, this method does nothing. It can be overridden if the client
             * wants to react to being closed.
             */
            virtual void onClose(){
                // This method is meant to be overridden.
            };

            /**
             * If the client ever established its connection to the server, this method
             * is called once right before the {@link #execute()} method returns or throws
             * an exception.
             * <p>
             * By default, this method does nothing. It can be overridden if the client
             * wants to react to the end of its lifecycle. This is a good method to
             * clean up resources.
             */
            virtual void onDisconnect(){
                // This method is meant to be overridden.
            };

            /**
             * This method is called if this client encounters a problem which does not
             * immediately require it to close but which may cause problems.
             * <p>
             * By default, this method prints the warning to standard error
             * 
             * @param message a description of the problem
             */
            virtual void onWarning(const std::string& message);

            /**
             * This method is called if the server reports an {@link Error error} to
             * this client.
             * <p>
             * By default, if the session has not yet started, this method throws a
             * std::runtime_error whose message is the message passed to this
             * method. If the session has started, the error will be printed to
             * standard error and no exception will be thrown.
             * 
             * @param message an explanation of what caused the error
             */
            virtual void onError(const std::string& message);

            /**
             * Makes an external call to a large language model API to complete a text
             * prompt.
             * <p>
             * This method requires the agent to have an {@link #ENVIRONMENT_VARIABLE_API_KEY API key}; 
             * without one, this method will throw an exception.
             * 
             * @param system the system prompt which instructs the language model how
             * to respond to the prompt
             * @param prompt the prompt which the large language model will respond to
             * @param temperature a parameter influencing the predictability of the
             * language model's output, where 0 means completely predictable and
             * higher values mean less predictable (more "creative") output
             * @return the response from the large language model to the prompt
             * @throws std::logic_error if the client does not have an API key
             */
            std::string complete(const std::string& system, const std::string& prompt, float temperature);

            /**
             * Makes an external call to a large language model API to embed a text
             * string in the model's latent space.
             * <p>
             * This method requires the agent to have an {@link #ENVIRONMENT_VARIABLE_API_KEY API key}; 
             * without one, this method will throw an exception.
             * 
             * @param string the string to embed
             * @param f an array that will be populated with a vector representation
             * of the string's embedding
             * @throws std::logic_error if the client does not have an API key
             */

            int embed(const std::string& string, float f[]);

            /**
             * Waits for a {@link tt::Message Message} of any type to be received.
             * <p>
             * It is not recommended for implementations of this abstract class to
             * call this function.
             * 
             * @return a unique_ptr to the message received.
             */
            std::unique_ptr<Message> receiveAny();

            template <typename T>
            /**
             * Waits for a {@link tt::Message Message} of a specified type to be received.
             * Throws an error if any other type is received instead.
             * <p>
             * It is not recommended for implementations of this abstract class to
             * call this function.
             * 
             * @param <T> the type of {@link tt::Message Message} to wait for
             * @return a unique_ptr to the message received
             * @throws std::runtime_error if the received message is of the wrong type
             */
            std::unique_ptr<T> receive();

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
            std::unique_ptr<Join> join;

            /** The secure socket used to connect to the server */
            SSL* ssl;
            SSL_CTX* ctx;
            int sock;
            
            /** Used to read messages received over the socket */
            std::thread receiveThread_;
            std::atomic<bool> running_{true};
            
            /** Whether the client has sent its join request yet */
            bool joined = false;
            
            /** The world in which this client's story takes places */
            std::unique_ptr<World> world = nullptr;
            
            /** This client's role in the story */
            Role role = Role::NONE;
            
            /** The current status of the story world as received from the server */
            std::unique_ptr<Status> status = nullptr;
            
            /** The current choices available to the client */
            std::vector<const Turn *> choices;
            
            /** The stop message received from the server */
            const Stop *stop = nullptr;
            
            /** The session ID received from the server at the end of the session */
            std::string session = "";

            /**
             * If the client has sent its join request, this method throws an exception
             * that explains why a property can no longer be modified.
             * 
             * @param property the name of the property which can no longer be modified
             * after joining the server
             * @throws std::logic_error if the client has already sent its join
             * request
             */
            void failIfJoined(const std::string& property);

            /**
             * If the object passed to this method is nullptr, this method throws an
             * exception explaining that it is not available because the session has
             * not started. Otherwise, the given object is returned.
             * 
             * @param <T> the type of object given as input and returned as output
             * @param object the object to be returned if it is not nullptr
             * @param description a description of the object
             * @return the object
             * @throws std::logic_error if the object is nullptr
             */
            template <typename T>
            static const T* failIfNotStarted(const T* object, const std::string& description);

            /**
             * If the object passed to this method is nullptr, this method throws an
             * exception explaining that it is not available because the session has
             * not started. Otherwise, the given object is returned.
             * 
             * @param <T> the type of object given as input and returned as output
             * @param object the object to be returned if it is not nullptr
             * @param description a description of the object
             * @return the object
             * @throws std::logic_error if the object is nullptr
             */
            template <typename T>
            static const T* failIfNotStarted(const std::unique_ptr<T>& object, const std::string& description);

            /**
             * This method sends a message to the server.
             * <p>
             * This function uses the SSL connection to send the message.
             * It is not recommended for implementations of this abstract class to
             * call this function.
             * 
             * @param s the message to send to the server
             */
            void sendMessage(const std::string& s);

            /**
             * Returns this client's {@link #key API key} or throws an exception if it
             * does not have one.
             * 
             * @return the client's API key
             * @throws std::logic_error if the client does not have an API key
             */
            const std::string& getKey() const;

            /**
             * This function constantly waits for messages to be sent. It places received
             * messages in the {@link #messageQueue_ message queue} to be processed.
             * <p>
             * It is usually unsafe for implementations of this abstract class to call
             * this method. Calling this method at an unexpected time is likely to cause 
             * this client to get out of sync with the server.
             */
            void receiveLoop();

            /* A blocking queue used by {@link #execute()} to wait for messages */
            BlockingQueue<std::unique_ptr<Message>> messageQueue_;
            // std::atomic<bool> running_{true};

            /**
             * This function deserializes raw json into a unique pointer for a message object
             * 
             * @param data a char array representing the json data
             * @param length the length of the char array
             * @return a unique_ptr to the deserialized message
             */
            std::unique_ptr<Message> processMessage(const char* data, int length);

            /* A buffer used by {@link #receiveLoop} to store messages if they are received in parts*/
            std::string recvBuffer_;
    };
    /**
     * Override stream insertion operator to allow printing the Client class
     * 
     * @param os an output stream to insert into
     * @param a a reference to a client object
     * @returns the output stream referenced
     */ 
    std::ostream& operator<<(std::ostream& os, const Client& a);
}

#endif