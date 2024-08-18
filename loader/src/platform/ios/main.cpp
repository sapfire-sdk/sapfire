#include <Sapfire/DefaultInclude.hpp>

#include "../load.hpp"
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <unistd.h>

#include <thread>

using namespace sapfire::prelude;

std::length_error::~length_error() _NOEXCEPT {} // do not ask...

// camila has an old ass macos and this function turned
// from dynamic to static thats why she needs to define it
// this is what old versions does to a silly girl

void dynamicEntry() {
    auto dylib = dlopen("SapfireBootstrapper.dylib", RTLD_NOLOAD);
    dlclose(dylib);

    auto workingDir = dirs::getGameDir();
    auto libDir = workingDir / "Frameworks";
    auto updatesDir = workingDir / "sapfire" / "update";

    auto error = std::error_code();

    if (std::filesystem::exists(updatesDir / "SapfireBootstrapper.dylib", error) && !error) {
        std::filesystem::rename(
            updatesDir / "SapfireBootstrapper.dylib", libDir / "SapfireBootstrapper.dylib", error
        );
        if (error) return;
    }

    sapfireEntry(nullptr);
}

extern "C" __attribute__((visibility("default"))) void dynamicTrigger() {
    std::thread(&dynamicEntry).detach();
}

// remove when we can figure out how to not remove it
auto dynamicTriggerRef = &dynamicTrigger;
