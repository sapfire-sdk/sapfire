#pragma once

#include "../SapfireStyle.hpp"
#include "../sources/ModSource.hpp"
#include <server/Server.hpp>

class DevListPopup : public SapfirePopup<ModSource const&> {
protected:
    ModSource m_source;
public:
    static DevListPopup* create(ModSource const&);
    void onClose(cocos2d::CCObject*);
private:
    bool setup(ModSource const&);
    void createList();
    void onMoreByThisDev(CCObject* sender);
};
