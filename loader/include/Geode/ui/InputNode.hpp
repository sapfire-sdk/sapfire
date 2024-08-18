#pragma once

#include <Sapfire/DefaultInclude.hpp>
#include <Sapfire/binding/CCTextInputNode.hpp>
#include <cocos2d.h>

namespace sapfire {
    class SAPFIRE_DLL InputNode : public cocos2d::CCMenuItem {
    protected:
        cocos2d::extension::CCScale9Sprite* m_bgSprite;
        CCTextInputNode* m_input;

        bool init(float, float, char const*, char const*, std::string const&, int);
        bool init(float, char const*, char const*, std::string const&, int);

    public:
        [[deprecated("Use sapfire::TextInput from the ui/TextInput.hpp header instead")]]
        static InputNode* create(
            float width, char const* placeholder, char const* fontFile, std::string const& filter,
            int limit
        );
        [[deprecated("Use sapfire::TextInput from the ui/TextInput.hpp header instead")]]
        static InputNode* create(
            float width, char const* placeholder, std::string const& filter, int limit
        );
        [[deprecated("Use sapfire::TextInput from the ui/TextInput.hpp header instead")]]
        static InputNode* create(float width, char const* placeholder, std::string const& filter);
        [[deprecated("Use sapfire::TextInput from the ui/TextInput.hpp header instead")]]
        static InputNode* create(float width, char const* placeholder, char const* fontFile);
        [[deprecated("Use sapfire::TextInput from the ui/TextInput.hpp header instead")]]
        static InputNode* create(float width, char const* placeholder);

        CCTextInputNode* getInput() const;
        cocos2d::extension::CCScale9Sprite* getBG() const;

        void setEnabled(bool enabled) override;

        void setString(std::string const&);
        std::string getString();
    };
}
