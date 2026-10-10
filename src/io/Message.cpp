#include <tt/io/Message.h>

using namespace tt;

const Client* Message::getClient() const {
    return client;
}

void Message::setClient(const Client* client)
{
    this->client = client;
}

std::string tt::Message::toString() const
{
    return std::string();
}

std::ostream &tt::operator<<(std::ostream &os, const Message &a)
{
    os << a.toString();
    return os;
}
