#include <tt/util/JsonUtil.h>

#pragma once

#ifndef UTIL_H
#define UTIL_H

// this is a space for any utility functions I need
namespace tt {
    std::string vtos(const Value &v) {
        if (auto* c = std::get_if<Constant>(&v))    return c->toString();
        if (auto* e = std::get_if<Entity>(&v))      return e->toString();
        return "Value of unknown type";
    }
}


#endif
