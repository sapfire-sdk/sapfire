#include <Sapfire/utils/terminate.hpp>
#include <Sapfire/loader/Mod.hpp>
#include "../loader/Log.hpp"

using namespace sapfire::prelude;

void sapfire::utils::detail::logTerminationError(const char* reason, Mod* mod) {
    // invoke logImpl directly to indicate what mod it came from
    log::logImpl(
        Severity::Error, mod,
        "The mod '{}' by {} has deliberately asked the game to crash.\nReason: {}",
        mod->getID(), fmt::format("{}", fmt::join(mod->getDevelopers(), ", ")), reason
    );
}
