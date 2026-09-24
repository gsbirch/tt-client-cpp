#include <tt/io/Message.h>

using namespace tt;

/**
 * Configures a {@link GsonBuilder} to encode and decode {@link Message}
 * objects as JSON.
 * 
 * @param builder the GSON builder to configure
 */
// void Message::configure(int builder) {
    // Read and write the various subclasses of Expression.
    // Expression.configure(builder);
    // // Read and write the various subclasses of Message.
    // AbstractAdapter<Message> adapter = new AbstractAdapter<>(Message.class);
    // builder.registerTypeAdapterFactory(adapter);
    // builder.registerTypeAdapter(Message.class, adapter);
    // // Serialize all implementations of worlds as generic World objects.
    // builder.registerTypeAdapterFactory(new GenericAdapter<>(World.class));
// }

/**
 * Constructs a message.
 */
Message::Message():
client(nullptr) {
    // Do nothing.
}

/**
 * Returns the {@link Agent} who sent this message, or null if this message
 * was sent by the server.
 * 
 * @return the agent who sent the message, or null
 */
const Client* Message::getClient() const {
    return client;
}

/**
 * Sets the {@link Agent} who sent this message. For messages sent to the
 * server, this method should be called soon after the message has been
 * parsed.
 * 
 * @param agent the agent who sent this message
 */
void Message::setClient(const Client* client)
{
    this->client = client;
}

std::string tt::Message::toString() const
{
    return std::string();
}

void tt::Message::verify() const
{
    // do nothing for now
}

std::ostream &tt::operator<<(std::ostream &os, const Message &a)
{
    os << a.toString();
    return os;
}
