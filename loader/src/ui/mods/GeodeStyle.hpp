#pragma once

#include <Sapfire/DefaultInclude.hpp>
#include <Sapfire/binding/ButtonSprite.hpp>
#include <Sapfire/ui/General.hpp>
#include <Sapfire/ui/IconButtonSprite.hpp>
#include <Sapfire/ui/BasedButtonSprite.hpp>
#include <Sapfire/ui/Popup.hpp>
#include <Sapfire/loader/Mod.hpp>

using namespace sapfire::prelude;

enum class SapfirePopupStyle {
    Default,
    Alt,
    Alt2,
};

template <class... Args>
class SapfirePopup : public Popup<Args...> {
protected:
    bool init(float width, float height, Args... args, SapfirePopupStyle style = SapfirePopupStyle::Default) {
        const bool sapfireTheme = Mod::get()->template getSettingValue<bool>("enable-sapfire-theme");
        const char* bg;
        switch (style) {
            default:
            case SapfirePopupStyle::Default: bg = sapfireTheme ? "GE_square01.png"_spr : "GJ_square01.png"; break;
            case SapfirePopupStyle::Alt:     bg = sapfireTheme ? "GE_square02.png"_spr : "GJ_square02.png"; break;
            case SapfirePopupStyle::Alt2:    bg = sapfireTheme ? "GE_square03.png"_spr : "GJ_square02.png"; break;
        }
        if (!Popup<Args...>::initAnchored(width, height, std::forward<Args>(args)..., bg))
            return false;
        
        this->setCloseButtonSpr(
            CircleButtonSprite::createWithSpriteFrameName(
                "close.png"_spr, .85f,
                (sapfireTheme ? 
                    (style == SapfirePopupStyle::Default ? CircleBaseColor::DarkPurple : CircleBaseColor::DarkAqua) : 
                    CircleBaseColor::Green
                )
            )
        );

        return true;
    }
};

class SapfireSquareSprite : public CCSprite {
protected:
    bool* m_stateSrc = nullptr;
    bool m_state = false;
    CCSprite* m_topSprite;

    bool init(CCSprite* top, bool* state);

    void update(float dt) override;
    void updateImage();

public:
    static SapfireSquareSprite* create(const char* top, bool* state = nullptr);
    static SapfireSquareSprite* createWithSpriteFrameName(const char* top, bool* state = nullptr);

    CCSprite* getTopSprite() const;
    void setState(bool state);
};

CCNode* createLoadingCircle(float sideLength, const char* id = "loading-spinner");

enum class SapfireButtonSprite {
    Default,
    Install,
    Delete,
    Enable,
};
const char* getSapfireButtonSpriteName(SapfireButtonSprite spr);
IconButtonSprite* createSapfireButton(CCNode* icon, std::string const& text, SapfireButtonSprite bg = SapfireButtonSprite::Default);
ButtonSprite* createSapfireButton(std::string const& text, int width, bool absolute = false, bool gold = false, SapfireButtonSprite bg = SapfireButtonSprite::Default);
ButtonSprite* createSapfireButton(std::string const& text, bool gold = false, SapfireButtonSprite bg = SapfireButtonSprite::Default);

CircleButtonSprite* createSapfireCircleButton(CCSprite* top, float scale = 1.f, CircleBaseSize size = CircleBaseSize::Medium, bool altColor = false);

ButtonSprite* createSapfireTagLabel(std::string const& text, std::optional<std::pair<ccColor3B, ccColor3B>> const& color = std::nullopt);
std::pair<ccColor3B, ccColor3B> sapfireTagColor(std::string_view const& text);

ListBorders* createSapfireListBorders(CCSize const& size);

bool isSapfireTheme();

class SapfireTabSprite : public CCNode {
protected:
    CCScale9Sprite* m_deselectedBG;
    CCScale9Sprite* m_selectedBG;
    CCSprite* m_icon;
    CCLabelBMFont* m_label;

    bool init(const char* iconFrame, const char* text, float width, bool altColor);

public:
    static SapfireTabSprite* create(const char* iconFrame, const char* text, float width, bool altColor = false);

    void select(bool selected);
    void disable(bool disabled);
};
