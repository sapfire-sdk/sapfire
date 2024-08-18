#include <Sapfire/modify/ProfilePage.hpp>
#include <Sapfire/binding/GJUserScore.hpp>

using namespace sapfire::prelude;

// Fixes the names not showing up on profile links
struct CustomProfilePage : Modify<CustomProfilePage, ProfilePage> {
    SAPFIRE_FORWARD_COMPAT_DISABLE_HOOKS("ProfilePage fix")

    virtual void getUserInfoFinished(GJUserScore* info) {
        m_usernameLabel->setString(info->m_userName.c_str());
        m_usernameLabel->limitLabelWidth(info->m_modBadge > 0 ? 140.f : 160.0f, 0.8f, 0.0f);
        ProfilePage::getUserInfoFinished(info);
    }
};