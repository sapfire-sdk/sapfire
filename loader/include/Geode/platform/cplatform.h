#pragma once

#ifdef _MSC_VER
    #pragma warning(disable : 4099) // type first seen as class
    #pragma warning(default : 4067)
    #pragma warning(disable : 4251) // dll-interface
    #pragma warning(disable : 4244) // narrowing conversion
#endif

// Set dllexport/dllimport to sapfire classes & functions

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64) && !defined(__CYGWIN__)
    #define SAPFIRE_WINDOWS(...) __VA_ARGS__
    #define SAPFIRE_IS_WINDOWS
    #define SAPFIRE_IS_DESKTOP
    #define SAPFIRE_PLATFORM_NAME "Windows"
    #define SAPFIRE_PLATFORM_EXTENSION ".dll"
    #define SAPFIRE_PLATFORM_SHORT_IDENTIFIER "win"
    #define CC_TARGET_OS_WIN32

    #if defined(WIN64) || defined(_WIN64) || defined(__WIN64) && !defined(__CYGWIN__)
        #define SAPFIRE_IS_WINDOWS64
        #define SAPFIRE_WINDOWS64(...) __VA_ARGS__
        #define SAPFIRE_WINDOWS32(...)
        #define SAPFIRE_CALL
    #else
        #define SAPFIRE_IS_WINDOWS32
        #define SAPFIRE_WINDOWS32(...) __VA_ARGS__
        #define SAPFIRE_WINDOWS64(...)
        #define SAPFIRE_CALL __stdcall
    #endif
#else
    #define SAPFIRE_WINDOWS(...)
    #define SAPFIRE_WINDOWS32(...)
    #define SAPFIRE_WINDOWS64(...)
#endif

#if defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define SAPFIRE_MACOS(...)
        #define SAPFIRE_INTEL_MAC(...)
        #define SAPFIRE_ARM_MAC(...)
        #define SAPFIRE_IOS(...) __VA_ARGS__
        #define SAPFIRE_IS_IOS
        #define SAPFIRE_IS_MOBILE
        #define SAPFIRE_PLATFORM_NAME "iOS"
        #define SAPFIRE_PLATFORM_EXTENSION ".ios.dylib"
        #define SAPFIRE_PLATFORM_SHORT_IDENTIFIER "ios"
        #define CC_TARGET_OS_IPHONE
    #else
        #define SAPFIRE_IOS(...)
        #define SAPFIRE_MACOS(...) __VA_ARGS__
        #define SAPFIRE_IS_MACOS
        #define SAPFIRE_IS_DESKTOP
        #define SAPFIRE_PLATFORM_EXTENSION ".dylib"
        #define CC_TARGET_OS_MAC

        #if TARGET_CPU_ARM64
            #define SAPFIRE_PLATFORM_NAME "MacArm"
            #define SAPFIRE_IS_ARM_MAC
            #define SAPFIRE_PLATFORM_SHORT_IDENTIFIER "mac-arm"
            #define SAPFIRE_ARM_MAC(...) __VA_ARGS__
            #define SAPFIRE_INTEL_MAC(...)
        #else
            #define SAPFIRE_PLATFORM_NAME "MacIntel"
            #define SAPFIRE_IS_INTEL_MAC
            #define SAPFIRE_PLATFORM_SHORT_IDENTIFIER "mac-intel"
            #define SAPFIRE_ARM_MAC(...)
            #define SAPFIRE_INTEL_MAC(...) __VA_ARGS__
        #endif
    #endif
    #define SAPFIRE_CALL
#else
    #define SAPFIRE_MACOS(...)
    #define SAPFIRE_IOS(...)
    #define SAPFIRE_INTEL_MAC(...)
    #define SAPFIRE_ARM_MAC(...)
#endif

// Android
#if defined(__ANDROID__)
    #define SAPFIRE_ANDROID(...) __VA_ARGS__
    #define SAPFIRE_IS_ANDROID
    #define SAPFIRE_IS_MOBILE
    #define SAPFIRE_CALL
    #define CC_TARGET_OS_ANDROID

    #if defined(__arm__)
        #define SAPFIRE_ANDROID32(...) __VA_ARGS__
		#define SAPFIRE_ANDROID64(...) 
        #define SAPFIRE_IS_ANDROID32
		#define SAPFIRE_PLATFORM_NAME "Android32"
        #define SAPFIRE_PLATFORM_EXTENSION ".android32.so"
        #define SAPFIRE_PLATFORM_SHORT_IDENTIFIER "android32"
	#elif defined(__aarch64__)
        #define SAPFIRE_ANDROID32(...)
		#define SAPFIRE_ANDROID64(...) __VA_ARGS__
        #define SAPFIRE_IS_ANDROID64
        #define SAPFIRE_PLATFORM_NAME "Android64"
        #define SAPFIRE_PLATFORM_EXTENSION ".android64.so"
        #define SAPFIRE_PLATFORM_SHORT_IDENTIFIER "android64"
    #else
        #define SAPFIRE_ANDROID32(...)
        #define SAPFIRE_ANDROID64(...)
	#endif
#else
    #define SAPFIRE_ANDROID(...)
    #define SAPFIRE_ANDROID32(...)
    #define SAPFIRE_ANDROID64(...)
#endif

#ifndef SAPFIRE_PLATFORM_NAME
    #error "Unsupported PlatformID!"
#endif
