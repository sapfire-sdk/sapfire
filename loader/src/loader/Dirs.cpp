
#include <Sapfire/loader/Dirs.hpp>
#include <cocos2d.h>
#include <crashlog.hpp>
#include <filesystem>
#include "LoaderImpl.hpp"

using namespace sapfire::prelude;

std::filesystem::path dirs::getSapfireDir() {
    return dirs::getGameDir() / "sapfire";
}

std::filesystem::path dirs::getSapfireSaveDir() {
    return dirs::getSaveDir() / "sapfire";
}

std::filesystem::path dirs::getSapfireResourcesDir() {
    return dirs::getSapfireDir() / "resources";
}

std::filesystem::path dirs::getSapfireLogDir() {
    return dirs::getSapfireDir() / "logs";
}

std::filesystem::path dirs::getTempDir() {
    return getSapfireDir() / "temp";
}

std::filesystem::path dirs::getModsDir() {
    return getSapfireDir() / "mods";
}

std::filesystem::path dirs::getModsSaveDir() {
    return getSapfireSaveDir() / "mods";
}

std::filesystem::path dirs::getModConfigDir() {
    return dirs::getSapfireDir() / "config";
}

std::filesystem::path dirs::getIndexDir() {
    return dirs::getSapfireDir() / "index";
}

std::filesystem::path dirs::getCrashlogsDir() {
    return crashlog::getCrashLogDirectory();
}
