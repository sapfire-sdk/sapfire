#pragma once

#include <Sapfire/DefaultInclude.hpp>
#include "MiniFunction.hpp"
#include <string_view>

namespace sapfire::utils::permission {
    enum class Permission {
        ReadAllFiles = 0x3, // maintain compatibility with some older mods
        RecordAudio
    };

    /**
     * Request whether the given permission is granted to Sapfire by the operating system.
     * @param permission The permission
    */
    bool SAPFIRE_DLL getPermissionStatus(Permission permission);

    /**
     * Request a permission to be granted by the operating system.
     * @param permission The permission
     * @param callback The callback, passed value is 'true' if permission was granted and 'false' otherwise.
     */
    void SAPFIRE_DLL requestPermission(Permission permission, utils::MiniFunction<void(bool)> callback);
}