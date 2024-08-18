#pragma once

#include <string>

namespace sapfire::utils::thread {
    // the platform-specific methods are needed for the thread names to show up
    // in places like task managers and debuggers
    void platformSetName(std::string const& name);
}
