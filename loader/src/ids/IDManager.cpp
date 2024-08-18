#include <Sapfire/modify/IDManager.hpp>

using namespace sapfire;

NodeIDs* NodeIDs::get() {
    static auto inst = new NodeIDs;
    return inst;
}
