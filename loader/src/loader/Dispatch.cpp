#include <Sapfire/loader/Dispatch.hpp>

using namespace sapfire::prelude;

std::unordered_map<std::string, EventListenerPool*>& sapfire::dispatchPools() {
    static std::unordered_map<std::string, EventListenerPool*> pools;
    return pools;
}