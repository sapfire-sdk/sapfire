#pragma once

#include <Sapfire/platform/platform.hpp>
#include <tulip/TulipHook.hpp>

namespace sapfire::hook {
    /**
     * Create a calling convention wrapper for a function.
     */
    SAPFIRE_DLL tulip::hook::Result<void*> createWrapper(
        void* address,
        tulip::hook::WrapperMetadata const& metadata
    ) noexcept;

    /**
     * Create an abstract calling convention handler for TulipHook
     */
    SAPFIRE_DLL std::shared_ptr<tulip::hook::CallingConvention> createConvention(
        tulip::hook::TulipConvention convention
    ) noexcept;
}
