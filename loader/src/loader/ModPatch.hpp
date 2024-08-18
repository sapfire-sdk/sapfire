#pragma once

#include <Sapfire/loader/Mod.hpp>

using namespace sapfire::prelude;

class ModPatch {
public:
    Mod* m_owner = nullptr;
    bool m_enabled = false;
    bool m_autoEnable = true;

    [[nodiscard]] Mod* getOwner() const;
    Result<> setOwner(Mod* mod);

    [[nodiscard]] bool isEnabled() const;
    virtual Result<> enable() = 0;
    virtual Result<> disable() = 0;

    [[nodiscard]] bool getAutoEnable() const;
    void setAutoEnable(bool autoEnable);
};
