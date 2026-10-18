# Tandem Tales Client (C++)

This library provides the abstract classes
tt::Client and tt::ClientFactory for implementing
[Tandem Tales](https://sgware.github.io/tt) agents in C++.

tt::Client handles connecting to the
[server](https://github.com/sgware/tt-server) and joining a session. A minimal
subclass of tt::Client only needs to implement the
tt::Client::onChoice method to choose what the agent will do on
its turn.

tt::ClientFactory continuously creates new clients to connect to the
server and wait for partners. A minimal subclass of tt::Factory only
needs to implement the tt::ClientFactory::create method to return a new
Client instance.

Here is an example client and factory:
```
#include <tt/Client.h>
#include <tt/ClientFactory.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

class MinimalClient : public tt::Client {
    public:
    MinimalClient(const std::string &url, int port):
    Client("random", "", tt::Role::NONE, "", url, port) {}

    int onChoice(const tt::Status* status) override {
        auto choices = status->getChoices();
        int count = choices.size();
        std::srand(std::time(nullptr));
        int choice = std::rand() % (count + 1);
        std::cout << choices[choice]->getDescription() << std::endl;
        return choice;
    }
};

class MinimalClientFactory : public tt::ClientFactory {
    public:
        std::unique_ptr<tt::Client> create() const override {
            return std::make_unique<MinimalClient>('localhost', 9005);
        }
};

int main() {
    auto factory = MinimalClientFactory();
    factory.execute();
}
```

Both classes have other lifecycle methods that are meant to be overridden.

tt::Client has the following lifecycle methods:

- tt::Client::onConnect, which runs after it connects to the
  server.
- tt::Client::onStart, which runs after its session starts.
- tt::Client::onUpdate, which runs each time the server sends a
  state update (even if it is not the client's turn).
- tt::Client::onChoice, which runs each time the server sends a
  state update and it is the client's turn.
- tt::Client::onEnd, which runs if the story session reaches a
  pre-defined ending.
- tt::Client::onClose, which runs after the client ends its session
  without raising an exception.
- tt::Client::onStop, which runs after the client's session has ended.
- tt::Client::onDisconnect, which runs after the client has
  disconnected.

tt::ClientFactory has the following lifecycle methods:

- @ref tt::ClientFactory::onStart() const "tt::ClientFactory::onStart", which runs after the factory starts.
- tt::ClientFactory::onClose, which runs after the factory has been shut
  down.
- tt::ClientFactory::onStop, which runs before the factory stops.

tt::Client also provides a way to call an external API for expensive
operations like text embedding and text completion. It does so through its
tt::Client::external member variable.

- tt::ExternalAPI::embed lets you embed a text string as an
  array of `float`s.
- tt::ExternalAPI::complete lets you complete a textconversation.

This [more complete example](https://github.com/gsbirch/tt-random-agent-cpp)
shows you everything you need to download and run a Tandem Tales server and
connect a custom C++ agent.