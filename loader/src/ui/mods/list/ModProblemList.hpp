#pragma once

#include <Sapfire/cocos/cocoa/CCGeometry.h>
#include <Sapfire/cocos/base_nodes/CCNode.h>
#include <Sapfire/loader/Loader.hpp>
#include <Sapfire/loader/Mod.hpp>
#include <Sapfire/ui/ScrollLayer.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

using namespace sapfire::prelude;

class ModProblemList : public CCNode {
protected:
    Mod* m_mod;
    ScrollLayer* m_list;
    CCScale9Sprite* m_bg;
public:
    static ModProblemList* create(
        Mod* mod,
        CCSize const& size
    );
protected:
    bool init(
        Mod* mod,
        CCSize const& size
    );
};
