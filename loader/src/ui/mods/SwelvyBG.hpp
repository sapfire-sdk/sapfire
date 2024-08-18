#pragma once

#include <Sapfire/ui/General.hpp>

using namespace sapfire::prelude;

class SwelvyBG : public CCNode {
protected:
    bool init();

    void updateSpritePosition(float dt);

public:
    static SwelvyBG* create();
};
