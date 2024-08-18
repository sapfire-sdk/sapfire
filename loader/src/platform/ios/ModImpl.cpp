#include <Sapfire/DefaultInclude.hpp>

#include <Sapfire/loader/Mod.hpp>
#include <loader/ModImpl.hpp>
#include <dlfcn.h>

using namespace sapfire::prelude;

template <typename T>
T findSymbolOrMangled(void* dylib, char const* name, char const* mangled) {
    auto res = reinterpret_cast<T>(dlsym(dylib, name));
    if (!res) {
        res = reinterpret_cast<T>(dlsym(dylib, mangled));
    }
    return res;
}

Result<> Mod::Impl::loadPlatformBinary() {
    auto dylib =
        dlopen((m_tempDirName / m_info.binaryName()).string().c_str(), RTLD_LAZY);
    if (dylib) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo { dylib };

        auto sapfireImplicitEntry = findSymbolOrMangled<void(*)()>(dylib, "sapfireImplicitEntry", "_Z17sapfireImplicitEntryv");
        if (sapfireImplicitEntry) {
            sapfireImplicitEntry();
        }

        auto sapfireCustomEntry = findSymbolOrMangled<void(*)()>(dylib, "sapfireCustomEntry", "_Z15sapfireCustomEntryv");
        if (sapfireCustomEntry) {
            sapfireCustomEntry();
        }

        return Ok();
    }
    std::string err = (char const*)dlerror();
    return Err("Unable to load the DYLIB: dlerror returned (" + err + ")");
}

Result<> Mod::Impl::unloadPlatformBinary() {
    auto dylib = m_platformInfo->m_dylib;
    delete m_platformInfo;
    m_platformInfo = nullptr;
    if (dlclose(dylib) == 0) {
        return Ok();
    }
    else {
        return Err("Unable to free library");
    }
}
