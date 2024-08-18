#pragma once

#include <Sapfire/ui/Popup.hpp>
#include "../UpdateModListState.hpp"

using namespace sapfire::prelude;

class ConfirmUninstallPopup : public Popup<Mod*> {
protected:
    Mod* m_mod;
    CCMenuItemToggler* m_deleteDataToggle;

    bool setup(Mod* mod) override;

    void onUninstall(CCObject*);

public:
    static ConfirmUninstallPopup* create(Mod* mod);
};
