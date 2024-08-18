#pragma once

#include <Sapfire/cocos/base_nodes/CCNode.h>
#include <Sapfire/cocos/cocoa/CCGeometry.h>
#include <Sapfire/cocos/sprite_nodes/CCSprite.h>
#include <Sapfire/loader/Loader.hpp>
#include <GUI/CCControlExtension/CCScale9Sprite.h>

using namespace sapfire::prelude;

class ModProblemItem : public CCNode {
protected:
    enum class Severity {
        Error,
        Info
    };
    LoadProblem m_problem;
    Mod* m_source;
    CCScale9Sprite* m_bg;
    ModProblemItem::Severity m_severity;
public:
    static ModProblemItem* create(Mod* source, LoadProblem problem, CCSize const& size);
protected:
    bool init(Mod* source, LoadProblem problem, CCSize const& size);
    ModProblemItem::Severity computeSeverity();
    bool showFixButton();
    bool showInfoButton();
    void onInfo(CCObject*);
    CCSprite* createSeverityIcon();
    std::string createProblemMessage();
};
