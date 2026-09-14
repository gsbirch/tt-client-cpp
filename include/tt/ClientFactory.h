#include <string>

#pragma once

#ifndef CLIENTFACTORY_H
#define CLIENTFACTORY_H

namespace tt {
    class ClientFactory {
        public:
            std::string factoryName;
            ClientFactory(std::string factoryName);
    };
}

#endif