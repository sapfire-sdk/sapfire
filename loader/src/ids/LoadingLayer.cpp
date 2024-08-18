#include <Sapfire/modify/IDManager.hpp>
#include <Sapfire/utils/NodeIDs.hpp>
#include <Sapfire/binding/LoadingLayer.hpp>

using namespace sapfire::prelude;
using namespace sapfire::node_ids;

$register_ids(LoadingLayer) {
    setIDs(
        this, 0,
        "bg-texture",
        "gd-logo",
        "robtop-logo",
        "cocos2d-logo",
        "fmod-logo",
        "loading-text",
        "text-area",
        "progress-slider"
    );
}
