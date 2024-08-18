#pragma once

#include <filesystem>
#include "../DefaultInclude.hpp"

namespace sapfire::dirs {
    /**
     * Directory where Geometry Dash is
     */
    SAPFIRE_DLL std::filesystem::path getGameDir();
    /**
     * Directory where GD saves its files
     */
    SAPFIRE_DLL std::filesystem::path getSaveDir();
    /**
     * Directory where Sapfire is
     */
    SAPFIRE_DLL std::filesystem::path getSapfireDir();
    /**
     * Directory where Sapfire saves its files
     */
    SAPFIRE_DLL std::filesystem::path getSapfireSaveDir();
    /**
     * Directory where Sapfire's resources are stored
     */
    SAPFIRE_DLL std::filesystem::path getSapfireResourcesDir();
    /**
     * Directory where Sapfire's resources are stored
     */
    SAPFIRE_DLL std::filesystem::path getSapfireLogDir();
    /**
     * Directory to store temporary files
     */
    SAPFIRE_DLL std::filesystem::path getTempDir();
    /**
     * Directory where mods are stored by default
     */
    SAPFIRE_DLL std::filesystem::path getModsDir();
    /**
     * Directory where mods' save data is stored
     */
    SAPFIRE_DLL std::filesystem::path getModsSaveDir();
    /**
     * Directory where mods' unzipped packages are stored at runtime
     */
    SAPFIRE_DLL std::filesystem::path getModRuntimeDir();
    /**
     * Directory where mods' config files lie
     */
    SAPFIRE_DLL std::filesystem::path getModConfigDir();
    /**
     * Directory where Sapfire stores the cached index
     */
    SAPFIRE_DLL std::filesystem::path getIndexDir();
    /**
     * Directory where crashlogs are stored
     */
    SAPFIRE_DLL std::filesystem::path getCrashlogsDir();
}
