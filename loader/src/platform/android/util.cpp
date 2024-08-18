using namespace sapfire::prelude;

#include <Sapfire/utils/cocos.hpp>
#include <Sapfire/loader/Dirs.hpp>
#include <Sapfire/utils/file.hpp>
#include <Sapfire/utils/web.hpp>
#include <filesystem>
#include <Sapfire/utils/general.hpp>
#include <Sapfire/utils/MiniFunction.hpp>
#include <Sapfire/utils/permission.hpp>
#include <Sapfire/utils/Task.hpp>
#include <Sapfire/loader/Loader.hpp>
#include <Sapfire/binding/AppDelegate.hpp>
#include <Sapfire/loader/Log.hpp>
#include <Sapfire/binding/MenuLayer.hpp>
#include <Sapfire/utils/Result.hpp>
#include <Sapfire/DefaultInclude.hpp>
#include <optional>
#include <mutex>

#include <jni.h>
#include <Sapfire/cocos/platform/android/jni/JniHelper.h>

using sapfire::utils::permission::Permission;

bool utils::clipboard::write(std::string const& data) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/sapfire/launcher/utils/SapfireUtils", "writeClipboard", "(Ljava/lang/String;)V")) {
        jstring stringArg1 = t.env->NewStringUTF(data.c_str());

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        return true;
    }
    return false;
}

std::string utils::clipboard::read() {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/sapfire/launcher/utils/SapfireUtils", "readClipboard", "()Ljava/lang/String;")) {
        jstring stringResult = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);

        std::string result = JniHelper::jstring2string(stringResult);

        t.env->DeleteLocalRef(stringResult);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
    return "";
}

CCPoint cocos::getMousePos() {
    return CCPoint(0, 0);
}

namespace {
    void clearJNIException() {
        // this is a silly workaround to not crash when the method is not found.
        // cocos figured this out half a year later...
        auto vm = JniHelper::getJavaVM();

        JNIEnv* env;
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_OK) {
            env->ExceptionClear();
        }
    }

    // jni breaks over multithreading, so the value is stored to avoid more jni calls
    std::string s_savedBaseDir = "";

    std::filesystem::path getBaseDir() {
        std::string path = "/storage/emulated/0/Android/data/com.sapfire.launcher/files";

        if (!s_savedBaseDir.empty()) {
            return std::filesystem::path(s_savedBaseDir);
        }

        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/sapfire/launcher/utils/SapfireUtils", "getBaseDirectory", "()Ljava/lang/String;")) {
            jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
            t.env->DeleteLocalRef(t.classID);
            path = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(str);
        } else {
            clearJNIException();
        }

        s_savedBaseDir = path;
        return std::filesystem::path(path);
    }
}

std::filesystem::path dirs::getGameDir() {
    return getBaseDir() / "game";
}

std::filesystem::path dirs::getSaveDir() {
    return getBaseDir() / "save";
}

std::filesystem::path dirs::getModRuntimeDir() {
    static std::string cachedResult = [] {
        // incase the jni fails, default to this
        std::string path = "/data/user/0/com.sapfire.launcher/files/";

        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "com/sapfire/launcher/utils/SapfireUtils", "getInternalDirectory", "()Ljava/lang/String;")) {
            jstring str = reinterpret_cast<jstring>(t.env->CallStaticObjectMethod(t.classID, t.methodID));
            t.env->DeleteLocalRef(t.classID);
            path = JniHelper::jstring2string(str);
            t.env->DeleteLocalRef(str);
        } else {
            clearJNIException();
        }

        return path;
    }();
    return std::filesystem::path(cachedResult) / "sapfire" / "unzipped";
}

void utils::web::openLinkInBrowser(std::string const& url) {
    CCApplication::sharedApplication()->openURL(url.c_str());
}

bool utils::file::openFolder(std::filesystem::path const& path) {
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/sapfire/launcher/utils/SapfireUtils", "openFolder", "(Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF(path.string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }
    return false;
}

std::mutex s_callbackMutex;
static utils::MiniFunction<void(Result<std::filesystem::path>)> s_fileCallback {};
static utils::MiniFunction<void(Result<std::vector<std::filesystem::path>>)> s_filesCallback {};
static utils::MiniFunction<bool()> s_taskCancelled {};

extern "C"
JNIEXPORT void JNICALL Java_com_sapfire_launcher_utils_SapfireUtils_selectFileCallback(
        JNIEnv *env,
        jobject,
        jstring data
) {
    auto isCopy = jboolean();
    auto dataStr = env->GetStringUTFChars(data, &isCopy);

    const std::lock_guard lock(s_callbackMutex);
    if (s_taskCancelled && s_taskCancelled()) {
        s_taskCancelled = {};
        return;
    }
    if (s_fileCallback) {
        s_fileCallback(Ok(std::filesystem::path(dataStr)));
        s_fileCallback = {};
        s_taskCancelled = {};
    }
}

extern "C"
JNIEXPORT void JNICALL Java_com_sapfire_launcher_utils_SapfireUtils_selectFilesCallback(
        JNIEnv *env,
        jobject,
        jobjectArray datas
) {
    auto isCopy = jboolean();
    auto count = env->GetArrayLength(datas);
    auto result = std::vector<std::filesystem::path>();
    for (int i = 0; i < count; i++) {
        auto data = (jstring)env->GetObjectArrayElement(datas, i);
        auto dataStr = env->GetStringUTFChars(data, &isCopy);
        result.push_back(dataStr);
    }
    const std::lock_guard lock(s_callbackMutex);
    if (s_taskCancelled && s_taskCancelled()) {
        s_taskCancelled = {};
        return;
    }
    if (s_filesCallback) {
        s_filesCallback(Ok(std::move(result)));
        s_filesCallback = {};
        s_taskCancelled = {};
    }
}

extern "C"
JNIEXPORT void JNICALL Java_com_sapfire_launcher_utils_SapfireUtils_failedCallback(
        JNIEnv *env,
        jobject
) {
    const std::lock_guard lock(s_callbackMutex);
    if (s_fileCallback) {
        s_fileCallback(Err("Permission error"));
        s_fileCallback = {};
    }
    if (s_filesCallback) {
        s_filesCallback(Err("Permission error"));
        s_filesCallback = {};
    }
    if (s_taskCancelled) {
        s_taskCancelled = {};
    }
}

Task<Result<std::filesystem::path>> file::pick(file::PickMode mode, file::FilePickOptions const& options) {
    using RetTask = Task<Result<std::filesystem::path>>;

    const std::lock_guard lock(s_callbackMutex);
    if (s_fileCallback || s_filesCallback || s_taskCancelled) {
        return RetTask::immediate(Err("File picker was already called this frame"));
    }

    std::string method;
    switch (mode) {
        case file::PickMode::OpenFile:
            method = "selectFile";
            break;
        case file::PickMode::SaveFile:
            method = "createFile";
            break;
        case file::PickMode::OpenFolder:
            method = "selectFolder";
            break;
    }

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/sapfire/launcher/utils/SapfireUtils", method.c_str(), "(Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF(options.defaultPath.value_or(std::filesystem::path()).filename().string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        if (!result) {
            return RetTask::immediate(Err("Failed to open file picker"));
        }
    }
    return RetTask::runWithCallback([] (auto result, auto progress, auto cancelled) {
        const std::lock_guard lock(s_callbackMutex);
        s_fileCallback = result;
        s_taskCancelled = cancelled;
    });
}

Task<Result<std::vector<std::filesystem::path>>> file::pickMany(FilePickOptions const& options) {
    using RetTask = Task<Result<std::vector<std::filesystem::path>>>;

    const std::lock_guard lock(s_callbackMutex);
    if (s_fileCallback || s_filesCallback || s_taskCancelled) {
        return RetTask::immediate(Err("File picker was already called this frame"));
    }

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/sapfire/launcher/utils/SapfireUtils", "selectFiles", "(Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF(options.defaultPath.value_or(std::filesystem::path()).string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        if (!result) {
            return RetTask::immediate(Err("Failed to open file dialog"));
        }
    }

    return RetTask::runWithCallback([options](auto result, auto progress, auto cancelled){
        const std::lock_guard lock(s_callbackMutex);
        s_filesCallback = result;
        s_taskCancelled = cancelled;
    });
}

void sapfire::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    log::error("Launching Sapfire uninstaller is not supported on android");
}

void sapfire::utils::game::exit() {
    // TODO: yeah
    // if (CCApplication::sharedApplication() &&
    //     (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
    //     log::error("Cannot exit in PlayLayer or LevelEditorLayer!");
    //     return;
    // }
    AppDelegate::get()->trySaveGame(true);
    // AppDelegate::get()->showLoadingCircle(false, true);

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(MenuLayer::endGame)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

void sapfire::utils::game::restart() {
    // if (CCApplication::sharedApplication() &&
    //     (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
    //     log::error("Cannot restart in PlayLayer or LevelEditorLayer!");
    //     return;
    // }

    class Exit : public CCObject {
    public:
        void restart() {
            JniMethodInfo t;
            if (JniHelper::getStaticMethodInfo(t, "com/sapfire/launcher/utils/SapfireUtils", "restartGame", "()V")) {
                t.env->CallStaticVoidMethod(t.classID, t.methodID);

                t.env->DeleteLocalRef(t.classID);
            }
        }
    };
    // Not implemented
    // log::error("Restarting the game is not implemented on android");

    AppDelegate::get()->trySaveGame(true);
    // AppDelegate::get()->showLoadingCircle(false, true);

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(Exit::restart)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

static const char* permissionToName(Permission permission) {
#define PERM(x) "android.permission." x
#define INTERNAL_PERM(x) "sapfire.permission_internal." x
    switch (permission) {
    case Permission::RecordAudio: return PERM("RECORD_AUDIO");
    case Permission::ReadAllFiles: return INTERNAL_PERM("MANAGE_ALL_FILES");
    }
#undef PERM
#undef INTERNAL_PERM
}

bool sapfire::utils::permission::getPermissionStatus(Permission permission) {
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info, "com/sapfire/launcher/utils/SapfireUtils", "getPermissionStatus", "(Ljava/lang/String;)Z")) {
        jstring permString = info.env->NewStringUTF(permissionToName(permission));
        jboolean result = info.env->CallStaticBooleanMethod(info.classID, info.methodID, permString);
        info.env->DeleteLocalRef(info.classID);
        info.env->DeleteLocalRef(permString);

        return result == JNI_TRUE;
    } else {
        clearJNIException();
    }

    return false;
}

static MiniFunction<void(bool)> s_permissionCallback;

extern "C"
JNIEXPORT void JNICALL Java_com_sapfire_launcher_utils_SapfireUtils_permissionCallback(
        JNIEnv* env,
        jobject,
        jboolean granted
) {
    if (s_permissionCallback) {
        Loader::get()->queueInMainThread([granted] {
            s_permissionCallback(granted == JNI_TRUE);
        });
    }
}

void sapfire::utils::permission::requestPermission(Permission permission, utils::MiniFunction<void(bool)> callback) {
    s_permissionCallback = callback;
    JniMethodInfo info;
    if (JniHelper::getStaticMethodInfo(info, "com/sapfire/launcher/utils/SapfireUtils", "requestPermission", "(Ljava/lang/String;)V")) {
        jstring permString = info.env->NewStringUTF(permissionToName(permission));
        info.env->CallStaticVoidMethod(info.classID, info.methodID, permString);
        info.env->DeleteLocalRef(info.classID);
        info.env->DeleteLocalRef(permString);
    } else {
        clearJNIException();
    }
}

#include "../../utils/thread.hpp"
#include <unistd.h>

std::string sapfire::utils::thread::getDefaultName() {
    return fmt::format("Thread #{}", gettid());
}

void sapfire::utils::thread::platformSetName(std::string const& name) {
    pthread_setname_np(pthread_self(), name.c_str());
}
