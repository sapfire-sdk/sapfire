#include <Sapfire/ui/SceneManager.hpp>

using namespace sapfire::prelude;


#ifdef SAPFIRE_IS_WINDOWS

#include <Sapfire/modify/AppDelegate.hpp>
struct SceneSwitch : Modify<SceneSwitch, AppDelegate> {
    SAPFIRE_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    void willSwitchToScene(CCScene* scene) {
        AppDelegate::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};

#else

#include <Sapfire/modify/AchievementNotifier.hpp>
struct SceneSwitch : Modify<SceneSwitch, AchievementNotifier> {
    SAPFIRE_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};

#endif
