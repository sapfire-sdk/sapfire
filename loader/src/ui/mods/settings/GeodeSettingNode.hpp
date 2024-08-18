#pragma once

#include <Sapfire/loader/Event.hpp>
#include <Sapfire/utils/Result.hpp>
#include <Sapfire/binding/CCMenuItemSpriteExtra.hpp>
#include <Sapfire/binding/CCTextInputNode.hpp>
#include <Sapfire/binding/Slider.hpp>
#include <Sapfire/binding/SliderThumb.hpp>
#include <Sapfire/loader/Setting.hpp>
#include <Sapfire/loader/SettingNode.hpp>
#include <Sapfire/ui/BasedButtonSprite.hpp>
#include <Sapfire/ui/ColorPickPopup.hpp>
#include <Sapfire/ui/InputNode.hpp>
#include <Sapfire/ui/Popup.hpp>
#include <Sapfire/utils/cocos.hpp>
#include <Sapfire/utils/string.hpp>
#include <Sapfire/loader/Mod.hpp>

using namespace sapfire::prelude;

#define IMPL_SETT_CREATE(type_) \
    static type_##SettingNode* create(              \
        type_##SettingValue* value, float width     \
    ) {                                             \
        auto ret = new type_##SettingNode;          \
        if (ret->init(value, width)) {       \
            ret->autorelease();                     \
            return ret;                             \
        }                                           \
        delete ret;                        \
        return nullptr;                             \
    }

template <class T>
class SapfireSettingNode : public SettingNode {
public:
    using ValueType = typename T::ValueType;

protected:
    float m_width;
    float m_height;
    ValueType m_uncommittedValue;
    CCMenu* m_menu;
    CCLabelBMFont* m_nameLabel;
    CCLabelBMFont* m_errorLabel;
    CCMenuItemSpriteExtra* m_resetBtn;

    bool init(SapfireSettingValue<T>* setting, float width) {
        if (!SettingNode::init(setting))
            return false;

        m_width = width;
        m_height = this->setupHeight(setting);
        this->setContentSize({ width, m_height });

        constexpr auto sidePad = 40.f;

        m_uncommittedValue = setting->getValue();

        auto name = setting->getDefinition().getDisplayName();

        m_nameLabel = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
        m_nameLabel->setAnchorPoint({ .0f, .5f });
        m_nameLabel->limitLabelWidth(width / 2 - 50.f, .5f, .1f);
        m_nameLabel->setPosition({ sidePad / 2, m_obContentSize.height / 2 });
        this->addChild(m_nameLabel);

        m_errorLabel = CCLabelBMFont::create("", "bigFont.fnt");
        m_errorLabel->setAnchorPoint({ .0f, .5f });
        m_errorLabel->limitLabelWidth(width / 2 - 50.f, .25f, .1f);
        m_errorLabel->setPosition({ sidePad / 2, m_obContentSize.height / 2 - 14.f });
        m_errorLabel->setColor({ 255, 100, 100 });
        m_errorLabel->setZOrder(5);
        this->addChild(m_errorLabel);

        m_menu = CCMenu::create();
        m_menu->setPosition({ m_obContentSize.width - sidePad / 2, m_obContentSize.height / 2 }
        );
        this->addChild(m_menu);

        float btnPos = 15.f;

        if (setting->castDefinition().description) {
            auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
            infoSpr->setScale(.6f);

            auto infoBtn = CCMenuItemSpriteExtra::create(
                infoSpr, this, menu_selector(SapfireSettingNode::onDescription)
            );
            infoBtn->setPosition(
                -m_obContentSize.width + sidePad + m_nameLabel->getScaledContentSize().width +
                    btnPos,
                0.f
            );
            m_menu->addChild(infoBtn);

            btnPos += 20.f;
        }

        auto resetBtnSpr = CCSprite::createWithSpriteFrameName("reset-gold.png"_spr);
        resetBtnSpr->setScale(.5f);

        m_resetBtn = CCMenuItemSpriteExtra::create(
            resetBtnSpr, this, menu_selector(SapfireSettingNode::onReset)
        );
        m_resetBtn->setPosition(
            -m_obContentSize.width + sidePad + m_nameLabel->getScaledContentSize().width +
                btnPos,
            .0f
        );
        m_menu->addChild(m_resetBtn);

        m_menu->setTouchEnabled(true);

        if (!this->setup(setting, width)) return false;

        this->valueChanged();

        return true;
    }

    void onDescription(CCObject*) {
        FLAlertLayer::create(
            setting()->getDefinition().getDisplayName().c_str(),
            setting()->castDefinition().description.value(),
            "OK"
        )->show();
    }

    void onReset(CCObject*) {
        createQuickPopup(
            "Reset",
            "Are you sure you want to <cr>reset</c> <cl>" + 
                setting()->getDefinition().getDisplayName() +
                "</c> to <cy>default</c>?",
            "Cancel", "Reset",
            [this](auto, bool btn2) {
                if (btn2) {
                    this->resetToDefault();
                }
            }
        );
    }

    virtual float setupHeight(SapfireSettingValue<T>* setting) const {
        return 40.f;
    }

    virtual bool setup(SapfireSettingValue<T>* setting, float width) = 0;

    virtual void valueChanged(bool updateText = true) {
        if (this->hasUncommittedChanges()) {
            m_nameLabel->setColor({0x11, 0xdd, 0x00});
        }
        else {
            m_nameLabel->setColor({0xff, 0xff, 0xff});
        }
        if (m_resetBtn) m_resetBtn->setVisible(this->hasNonDefaultValue());
        auto isValid = setting()->validate(m_uncommittedValue);
        if (!isValid) {
            m_errorLabel->setVisible(true);
            m_errorLabel->setString(isValid.unwrapErr().c_str());
        }
        else {
            m_errorLabel->setVisible(false);
        }
        this->dispatchChanged();
    }

    SapfireSettingValue<T>* setting() {
        return static_cast<SapfireSettingValue<T>*>(m_value);
    }

public:
    void commit() override {
        setting()->setValue(m_uncommittedValue);
        m_uncommittedValue = setting()->getValue();
        this->valueChanged();
        this->dispatchCommitted();
    }

    bool hasUncommittedChanges() override {
        return m_uncommittedValue != setting()->getValue();
    }

    bool hasNonDefaultValue() override {
        return m_uncommittedValue != setting()->castDefinition().defaultValue;
    }

    void resetToDefault() override {
        m_uncommittedValue = setting()->castDefinition().defaultValue;
        this->valueChanged();
    }
};

class BoolSettingNode : public SapfireSettingNode<BoolSetting> {
protected:
    CCMenuItemToggler* m_toggle;

    void onToggle(CCObject*);
    void valueChanged(bool updateText) override;
    bool setup(BoolSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(Bool);
};

class IntSettingNode :
    public SapfireSettingNode<IntSetting>,
    public TextInputDelegate
{
protected:
    TextInput* m_input = nullptr;
    CCLabelBMFont* m_label = nullptr;
    Slider* m_slider = nullptr;
    CCMenuItemSpriteExtra* m_decArrow = nullptr;
    CCMenuItemSpriteExtra* m_incArrow = nullptr;
    CCMenuItemSpriteExtra* m_bigDecArrow = nullptr;
    CCMenuItemSpriteExtra* m_bigIncArrow = nullptr;

    void valueChanged(bool updateText) override;
    void textChanged(CCTextInputNode* input) override;

    float setupHeight(IntSettingValue* setting) const override;
    bool setup(IntSettingValue* setting, float width) override;

public:
    void updateSlider();
    void updateLabel();

    void onSlider(CCObject* slider);
    void onArrow(CCObject* sender);

    IMPL_SETT_CREATE(Int);
};

class FloatSettingNode :
    public SapfireSettingNode<FloatSetting>,
    public TextInputDelegate
{
protected:
    TextInput* m_input = nullptr;
    CCLabelBMFont* m_label = nullptr;
    Slider* m_slider = nullptr;
    CCMenuItemSpriteExtra* m_decArrow = nullptr;
    CCMenuItemSpriteExtra* m_incArrow = nullptr;
    CCMenuItemSpriteExtra* m_bigDecArrow = nullptr;
    CCMenuItemSpriteExtra* m_bigIncArrow = nullptr;

    void valueChanged(bool updateText) override;
    void textChanged(CCTextInputNode* input) override;

    float setupHeight(FloatSettingValue* setting) const override;
    bool setup(FloatSettingValue* setting, float width) override;

public:
    void updateSlider();
    void updateLabel();
    
    void onSlider(CCObject* slider);
    void onArrow(CCObject* sender);

    IMPL_SETT_CREATE(Float);
};

class StringSettingNode :
    public SapfireSettingNode<StringSetting>,
    public TextInputDelegate
{
protected:
    TextInput* m_input = nullptr;
    CCLabelBMFont* m_label = nullptr;
    CCMenuItemSpriteExtra* m_prevBtn = nullptr;
    CCMenuItemSpriteExtra* m_nextBtn = nullptr;
    std::vector<std::string> m_options;
    int m_selectedOption = 0;
    float m_width;

    void textChanged(CCTextInputNode* input) override;
    void valueChanged(bool updateText) override;
    void updateLabel();
    
    void onArrow(CCObject* sender);

    bool setup(StringSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(String);
};

class FileSettingNode :
    public SapfireSettingNode<FileSetting>,
    public TextInputDelegate
{
protected:
    using FileTask = Task<Result<std::filesystem::path>>;

    TextInput* m_input;
    EventListener<FileTask> m_pickListener;

    void textChanged(CCTextInputNode* input) override;
    void valueChanged(bool updateText) override;
    void updateLabel();

    void onPickFile(CCObject*);
    void onPickFileFinished(FileTask::Event* event);

    bool setup(FileSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(File);
};

class ColorSettingNode :
    public SapfireSettingNode<ColorSetting>,
    public ColorPickPopupDelegate {
protected:
    ColorChannelSprite* m_colorSpr;

    void valueChanged(bool updateText) override;
    void updateColor(ccColor4B const& color) override;

    void onSelectColor(CCObject*);

    bool setup(ColorSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(Color);
};

class ColorAlphaSettingNode :
    public SapfireSettingNode<ColorAlphaSetting>,
    public ColorPickPopupDelegate {
protected:
    ColorChannelSprite* m_colorSpr;

    void valueChanged(bool updateText) override;
    void updateColor(ccColor4B const& color) override;

    void onSelectColor(CCObject*);

    bool setup(ColorAlphaSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(ColorAlpha);
};

class CustomSettingPlaceholderNode : public SettingNode {
protected:
    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    void resetToDefault() override;

    bool init(std::string const& key, float width);

public:
    static CustomSettingPlaceholderNode* create(
        std::string const& key, float width
    );
};
