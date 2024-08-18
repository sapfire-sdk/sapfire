#pragma once

#include <Sapfire/cocos/base_nodes/CCNode.h>
#include <Sapfire/cocos/cocoa/CCGeometry.h>
#include <Sapfire/cocos/sprite_nodes/CCSprite.h>
#include <Sapfire/loader/Loader.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>
#include <optional>
#include "ui/mods/popups/DevPopup.hpp"

using namespace sapfire::prelude;

class ModDeveloperItem : public CCNode {
protected:
    CCScale9Sprite* m_bg;
    DevListPopup* m_popup;
public:
    static ModDeveloperItem* create(
        DevListPopup* popup,
        std::string developer,
        CCSize const& size,
        std::optional<std::string> displayName = std::nullopt,
        bool addMoreButton = true
    );
private:
    bool init(
        DevListPopup* popup,
        std::string developer,
        CCSize const& size,
        std::optional<std::string> displayName,
        bool addMoreButton
    );
    void onMoreByThisDev(CCObject* sender);
};
