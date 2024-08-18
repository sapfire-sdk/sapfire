#include <Sapfire/loader/Loader.hpp>

using namespace sapfire::prelude;

#include <Sapfire/modify/AppDelegate.hpp>
#include <Sapfire/modify/CCApplication.hpp>

namespace {
    void saveModData() {
        log::info("Saving mod data...");
        log::pushNest();

        auto begin = std::chrono::high_resolution_clock::now();

        (void)Loader::get()->saveData();

        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        log::info("Took {}s", static_cast<float>(time) / 1000.f);

        log::popNest();
    }
}

struct SaveLoader : Modify<SaveLoader, AppDelegate> {
    SAPFIRE_FORWARD_COMPAT_DISABLE_HOOKS("save moved to CCApplication::gameDidSave()")
    void trySaveGame(bool p0) {
        saveModData();
        return AppDelegate::trySaveGame(p0);
    }
};

#ifdef SAPFIRE_IS_WINDOWS

struct FallbackSaveLoader : Modify<FallbackSaveLoader, CCApplication> {
    SAPFIRE_FORWARD_COMPAT_ENABLE_HOOKS("")
    void gameDidSave() {
        saveModData();
        return CCApplication::gameDidSave();
    }
};

#endif

#ifdef SAPFIRE_IS_ANDROID

#include <Sapfire/modify/FileOperation.hpp>
#include <Sapfire/loader/Dirs.hpp>

// redirects the save path to what sapfire knows, in case launcher's fopen hook fails
struct FileOperationOverride : Modify<FileOperationOverride, FileOperation> {
    static gd::string getFilePath() {
        return dirs::getSaveDir().string() + "/";
    }
};

#endif
