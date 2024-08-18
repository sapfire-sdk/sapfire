#pragma once

#include <dlfcn.h>
#include <type_traits>
#include "ItaniumCast.hpp"

namespace sapfire {
    struct PlatformInfo {
        void* m_so;
    };
}

namespace sapfire::base {
    /*SAPFIRE_NOINLINE inline*/ uintptr_t get();
}

