#pragma once

#include <Sapfire/platform/platform.hpp>

#include "string.hpp"

#if defined(SAPFIRE_IS_ANDROID)
#include "gnustl.hpp"
#else
#include "aliastl.hpp"
#endif
