#include <Sapfire/loader/SettingNode.hpp>
#include <Sapfire/utils/cocos.hpp>

using namespace sapfire::prelude;

void SettingNode::dispatchChanged() {
    if (m_delegate) {
        m_delegate->settingValueChanged(this);
    }
}

void SettingNode::dispatchCommitted() {
    if (m_delegate) {
        m_delegate->settingValueCommitted(this);
    }
}

bool SettingNode::init(SettingValue* value) {
    m_value = value;
    return true;
}

void SettingNode::setDelegate(SettingNodeDelegate* delegate) {
    m_delegate = delegate;
}
