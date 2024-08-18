#include <cocos2d.h>
#include <Sapfire/utils/cocos.hpp>
#include <Sapfire/utils/ranges.hpp>
#include <Sapfire/loader/Log.hpp>
#include <Sapfire/binding/CCMenuItemSpriteExtra.hpp>
#include <Sapfire/binding/CCMenuItemToggler.hpp>

using namespace sapfire::prelude;

bool SpacerNode::init(size_t grow) {
    if (!CCNode::init())
        return false;
    
    m_grow = grow;

    return true;
}

SpacerNode* SpacerNode::create(size_t grow) {
    auto ret = new SpacerNode;
    if (ret->init(grow)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void SpacerNode::setGrow(size_t grow) {
    m_grow = grow;
}

size_t SpacerNode::getGrow() const {
    return m_grow;
}

bool SpacerNodeChild::init(CCNode* child, size_t grow) {
    if (!SpacerNode::init(grow))
        return false;
    
    if (child) {
        this->addChild(child);
        m_child = child;
    }

    return true;
}

SpacerNodeChild* SpacerNodeChild::create(CCNode* child, size_t grow) {
    auto ret = new SpacerNodeChild;
    if (ret->init(child, grow)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void SpacerNodeChild::setContentSize(CCSize const& size) {
    CCNode::setContentSize(size);
    if (m_child) {
        m_child->setPosition(CCPointZero);
        m_child->setContentSize(size);
        m_child->setAnchorPoint(CCPointZero);
    }
}
