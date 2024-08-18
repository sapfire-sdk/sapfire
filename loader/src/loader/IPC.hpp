#pragma once

#include <string>
#include <matjson.hpp>

namespace sapfire::ipc {
    void setup();
    matjson::Value processRaw(void* rawHandle, std::string const& buffer);
}
