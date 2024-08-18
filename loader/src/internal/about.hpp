#pragma once

#include <Sapfire/utils/VersionInfo.hpp>

namespace about {
    SAPFIRE_DLL sapfire::VersionInfo getLoaderVersion();
    SAPFIRE_DLL const char* getLoaderVersionStr();
    SAPFIRE_DLL const char* getLoaderCommitHash();
    SAPFIRE_DLL const char* getBindingsCommitHash();
    SAPFIRE_DLL const char* getLoaderModJson();
};
