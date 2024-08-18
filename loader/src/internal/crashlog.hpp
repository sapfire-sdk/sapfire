#pragma once

#include <Sapfire/Loader.hpp>
#include <filesystem>
#include <string>

/**
 * Platform-specific crashlog functions. Used by the loader
 */
namespace crashlog {
    /**
     * Setup platform-specific crashlog handler
     * @returns True if the handler was successfully installed, false otherwise
     */
    bool SAPFIRE_DLL setupPlatformHandler();

    /**
     * Setup platform-specific crashlog handler for post-launch
     */
    void SAPFIRE_DLL setupPlatformHandlerPost();

    /**
     * Check if previous launch of GD crashed unexpectedly
     * @returns True if the launch crashed, false otherwise or if indeterminate
     */
    bool SAPFIRE_DLL didLastLaunchCrash();
    /**
     * Get the directory where crashlogs are saved on the current platform
     * @returns Path to the directory, or an empty string if the platform does
     * not support crash logs
     */
    std::filesystem::path SAPFIRE_DLL getCrashLogDirectory();

    std::string SAPFIRE_DLL writeCrashlog(sapfire::Mod* faultyMod, std::string const& info, std::string const& stacktrace, std::string const& registers);

    std::string writeCrashlog(sapfire::Mod* faultyMod, std::string const& info, std::string const& stacktrace, std::string const& registers, std::filesystem::path& outCrashlogPath);

    std::string getDateString(bool filesafe);

    void SAPFIRE_DLL printSapfireInfo(std::stringstream& stream);
    void SAPFIRE_DLL printMods(std::stringstream& stream);


}
