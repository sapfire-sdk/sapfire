#include <Sapfire/DefaultInclude.hpp>
#include <loader/IPC.hpp>
#include <Sapfire/loader/Log.hpp>

using namespace sapfire::prelude;

void ipc::setup() {
    log::debug("IPC is not supported on this platform!");
}
