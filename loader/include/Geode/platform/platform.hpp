#pragma once

#include "cplatform.h"
#include <string>
#include <functional>

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
    #define SAPFIRE_PRETTY_FUNCTION std::string(__FUNCSIG__)
#else
    #define SAPFIRE_PRETTY_FUNCTION std::string(__PRETTY_FUNCTION__)
#endif

// Windows
#ifdef SAPFIRE_IS_WINDOWS

    #define SAPFIRE_HIDDEN
    #define SAPFIRE_INLINE __forceinline
    #define SAPFIRE_VIRTUAL_CONSTEXPR
    #define SAPFIRE_NOINLINE __declspec(noinline)

    #ifdef SAPFIRE_EXPORTING
        #define SAPFIRE_DLL __declspec(dllexport)
    #else
        #define SAPFIRE_DLL __declspec(dllimport)
    #endif

    #define SAPFIRE_API extern "C" __declspec(dllexport)
    #define SAPFIRE_EXPORT __declspec(dllexport)

    #if defined(SAPFIRE_IS_WINDOWS64)
        #define SAPFIRE_IS_X64
        #define SAPFIRE_CDECL_CALL
    #else 
        #define SAPFIRE_IS_X86
        #define SAPFIRE_CDECL_CALL __cdecl
	#endif

    #include "windows.hpp"

#elif defined(SAPFIRE_IS_MACOS)

    #define SAPFIRE_HIDDEN __attribute__((visibility("hidden")))
    #define SAPFIRE_INLINE inline __attribute__((always_inline))
    #define SAPFIRE_VIRTUAL_CONSTEXPR constexpr
    #define SAPFIRE_NOINLINE __attribute__((noinline))

    #ifdef SAPFIRE_EXPORTING
        #define SAPFIRE_DLL __attribute__((visibility("default")))
    #else
        #define SAPFIRE_DLL
    #endif

    #define SAPFIRE_API extern "C" __attribute__((visibility("default")))
    #define SAPFIRE_EXPORT __attribute__((visibility("default")))

    #define SAPFIRE_IS_X64
    #define SAPFIRE_CDECL_CALL

    #include "macos.hpp"

#elif defined(SAPFIRE_IS_IOS)

    #define SAPFIRE_HIDDEN __attribute__((visibility("hidden")))
    #define SAPFIRE_INLINE inline __attribute__((always_inline))
    #define SAPFIRE_VIRTUAL_CONSTEXPR constexpr
    #define SAPFIRE_NOINLINE __attribute__((noinline))

    #ifdef SAPFIRE_EXPORTING
        #define SAPFIRE_DLL __attribute__((visibility("default")))
    #else
        #define SAPFIRE_DLL
    #endif

    #define SAPFIRE_API extern "C" __attribute__((visibility("default")))
    #define SAPFIRE_EXPORT __attribute__((visibility("default")))

    #define SAPFIRE_IS_X64
    #define SAPFIRE_CDECL_CALL

    #include "ios.hpp"

#elif defined(SAPFIRE_IS_ANDROID)

    #define SAPFIRE_HIDDEN __attribute__((visibility("hidden")))
    #define SAPFIRE_INLINE inline __attribute__((always_inline))
    #define SAPFIRE_VIRTUAL_CONSTEXPR constexpr
    #define SAPFIRE_NOINLINE __attribute__((noinline))

    #ifdef SAPFIRE_EXPORTING
        #define SAPFIRE_DLL __attribute__((visibility("default")))
    #else
        #define SAPFIRE_DLL
    #endif

    #define SAPFIRE_API extern "C" __attribute__((visibility("default")))
    #define SAPFIRE_EXPORT __attribute__((visibility("default")))

    #if defined(SAPFIRE_IS_ANDROID64)
        #define SAPFIRE_IS_X64
    #else 
        #define SAPFIRE_IS_X86
    #endif
    #define SAPFIRE_CDECL_CALL

    #include "android.hpp"

#else

    #error "Unsupported Platform!"

#endif

namespace sapfire {
    class PlatformID {
    public:
        enum {
            Unknown = -1,
            Windows,
            MacIntel,
            MacArm,
            iOS,
            Android32,
            Android64,
            Linux,
        };

        using Type = decltype(Unknown);

        Type m_value;

        constexpr PlatformID(Type t) {
            m_value = t;
        }

        constexpr PlatformID& operator=(Type t) {
            m_value = t;
            return *this;
        }

        constexpr bool operator==(int other) const {
            return m_value == other;
        }

        constexpr bool operator==(PlatformID const& other) const {
            return m_value == other.m_value;
        }

        constexpr bool operator<(PlatformID const& other) const {
            return m_value < other.m_value;
        }

        constexpr bool operator>(PlatformID const& other) const {
            return m_value > other.m_value;
        }

        constexpr operator int() const {
            return m_value;
        }

        /**
         * Parse string into PlatformID. String should be all-lowercase, for 
         * example "windows" or "linux"
         */
        static SAPFIRE_DLL PlatformID from(const char* str);
        static SAPFIRE_DLL PlatformID from(std::string const& str);

        /**
         * Determines if a given platform string "covers" the given platform.
         * For example, "android" is covered by Platform::Android32 and Platform::Android64.
         * Input string must follow the format in PlatformID::toShortString.
         */
        static SAPFIRE_DLL bool coveredBy(const char* str, PlatformID t);
        static SAPFIRE_DLL bool coveredBy(std::string const& str, PlatformID t);

        static constexpr char const* toString(Type lp) {
            switch (lp) {
                case Unknown: return "Unknown";
                case Windows: return "Windows";
                case MacIntel: return "MacIntel";
                case MacArm: return "MacArm";
                case iOS: return "iOS";
                case Android32: return "Android32";
                case Android64: return "Android64";
                case Linux: return "Linux";
                default: break;
            }
            return "Undefined";
        }

        static constexpr char const* toShortString(Type lp, bool ignoreArch = false) {
            switch (lp) {
                case Unknown: return "unknown";
                case Windows: return "win";
                case MacIntel: return ignoreArch ? "mac" : "mac-intel";
                case MacArm: return ignoreArch ? "mac" : "mac-arm";
                case iOS: return "ios";
                case Android32: return ignoreArch ? "android" : "android32";
                case Android64: return ignoreArch ? "android" : "android64";
                case Linux: return "linux";
                default: break;
            }
            return "undefined";
        }

        template <class T>
            requires requires(T t) {
                static_cast<Type>(t);
            }
        constexpr static PlatformID from(T t) {
            return static_cast<Type>(t);
        }

        template <class T>
            requires requires(Type t) {
                static_cast<T>(t);
            }
        constexpr T to() const {
            return static_cast<T>(m_value);
        }
    };
}

namespace std {
    template <>
    struct hash<sapfire::PlatformID> {
        inline std::size_t operator()(sapfire::PlatformID const& id) const {
            return std::hash<sapfire::PlatformID::Type>()(id.m_value);
        }
    };
}

#ifdef SAPFIRE_IS_WINDOWS
    #define SAPFIRE_PLATFORM_TARGET PlatformID::Windows
#elif defined(SAPFIRE_IS_ARM_MAC)
    #define SAPFIRE_PLATFORM_TARGET PlatformID::MacArm
#elif defined(SAPFIRE_IS_INTEL_MAC)
    #define SAPFIRE_PLATFORM_TARGET PlatformID::MacIntel
#elif defined(SAPFIRE_IS_IOS)
    #define SAPFIRE_PLATFORM_TARGET PlatformID::iOS
#elif defined(SAPFIRE_IS_ANDROID32)
    #define SAPFIRE_PLATFORM_TARGET PlatformID::Android32
#elif defined(SAPFIRE_IS_ANDROID64)
    #define SAPFIRE_PLATFORM_TARGET PlatformID::Android64
#endif
