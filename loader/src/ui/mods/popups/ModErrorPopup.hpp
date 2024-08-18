#pragma once

#include <Sapfire/loader/Mod.hpp>

#include "ui/mods/SapfireStyle.hpp"
#include "ui/mods/list/ModProblemList.hpp"

using namespace sapfire::prelude;

class ModErrorPopup : public SapfirePopup<Mod*> {
protected:
    Mod* m_mod;
    ModProblemList* m_list;
public:
    static ModErrorPopup* create(Mod* mod);

private:
    bool setup(Mod* mod) override;

    void createList();
};
