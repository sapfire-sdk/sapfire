#include <Sapfire/DefaultInclude.hpp>
#include <Sapfire/modify/LevelPage.hpp>

using namespace sapfire::prelude;

// When pressing spacebar during the transition into LevelSelectLayer,
// a PlayLayer will be created but it will never be added to a scene.
// It will still exist in memory, potentially causing issues in mods, i.e. Custom Keybinds.
//
// This workaround solves the issue by making it impossible to start the level during a transition.

struct CustomLevelPage : Modify<CustomLevelPage, LevelPage> {
    SAPFIRE_FORWARD_COMPAT_DISABLE_HOOKS("Impostor PlayLayer fix")

    void onPlay(cocos2d::CCObject* sender) {
        if (!typeinfo_cast<CCTransitionScene*>(CCScene::get())) {
            LevelPage::onPlay(sender);
        }
    }
};
