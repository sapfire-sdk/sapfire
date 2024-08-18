#include <Sapfire/Sapfire.hpp>
#include <Sapfire/modify/CCMenuItem.hpp>
#include <Sapfire/modify/CCMenu.hpp>

using namespace sapfire::prelude;

class $modify(CCMenuItem) {
    void activate() {
        this->retain();
        CCMenuItem::activate();
        this->release();
    }
};

class $modify(CCMenu) {
    void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        this->retain();
        CCMenu::ccTouchEnded(touch, event);
        this->release();
    }
};