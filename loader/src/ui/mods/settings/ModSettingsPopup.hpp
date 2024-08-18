#pragma once

#include <Sapfire/loader/SettingNode.hpp>
#include <Sapfire/ui/Popup.hpp>
#include <Sapfire/utils/cocos.hpp>
#include "../SapfireStyle.hpp"

using namespace sapfire::prelude;

class ModSettingsPopup : public SapfirePopup<Mod*>, public SettingNodeDelegate {
protected:
    Mod* m_mod;
    std::vector<SettingNode*> m_settings;
    CCMenuItemSpriteExtra* m_applyBtn;
    ButtonSprite* m_applyBtnSpr;

    void settingValueChanged(SettingNode*) override;
    void settingValueCommitted(SettingNode*) override;

    bool setup(Mod* mod) override;
    bool hasUncommitted() const;
    void onClose(CCObject*) override;
    void onApply(CCObject*);
    void onResetAll(CCObject*);
    void onOpenSaveDirectory(CCObject*);

public:
    static ModSettingsPopup* create(Mod* mod);
};
