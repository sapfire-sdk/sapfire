#include <Sapfire/utils/general.hpp>

#ifndef SAPFIRE_IS_MACOS
// feel free to properly implement this for other platforms
float sapfire::utils::getDisplayFactor() {
    return 1.0f;
}
#endif