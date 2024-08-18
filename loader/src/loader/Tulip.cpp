
#include <Sapfire/loader/Tulip.hpp>

tulip::hook::Result<void*> sapfire::hook::createWrapper(
    void* address,
    tulip::hook::WrapperMetadata const& metadata
) noexcept {
    return tulip::hook::createWrapper(address, metadata);
}

std::shared_ptr<tulip::hook::CallingConvention> sapfire::hook::createConvention(
    tulip::hook::TulipConvention convention
) noexcept {
    return tulip::hook::createConvention(convention);
}
