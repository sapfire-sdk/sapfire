#pragma once

#include <Sapfire/Prelude.hpp>
#include <Sapfire/c++stl/gdstdlib.hpp>
#include <Sapfire/platform/platform.hpp>
#include <variant>

#define SAPFIRE_STATIC_PTR(type, name)          \
    static type* s_##name;                    \
    inline type* name() {                     \
        if (!s_##name) s_##name = new type(); \
        return s_##name;                      \
    }

#define SAPFIRE_STATIC_VAR(type, name) \
    inline type& name() {            \
        static type s_##name;        \
        return s_##name;             \
    }

#define SAPFIRE_WRAPPER_CONCAT(x, y) x##y
#define SAPFIRE_CONCAT(x, y) SAPFIRE_WRAPPER_CONCAT(x, y)

#define SAPFIRE_WRAPPER_STR(...) #__VA_ARGS__
#define SAPFIRE_STR(...) SAPFIRE_WRAPPER_STR(__VA_ARGS__)

#define SAPFIRE_PAD(size) uint8_t SAPFIRE_CONCAT(__pad, __LINE__)[size]
#define SAPFIRE_UNIMPLEMENTED_PAD private:

#define SAPFIRE_NONINHERITED_MEMBERS private:

#define SAPFIRE_EXPAND(x) x
#define SAPFIRE_INVOKE(macro, ...) SAPFIRE_EXPAND(macro(__VA_ARGS__))

namespace sapfire {
    struct ZeroConstructorType {};

    static constexpr auto ZeroConstructor = ZeroConstructorType();

    struct CutoffConstructorType {};

    static constexpr auto CutoffConstructor = CutoffConstructorType();
}

#define SAPFIRE_CUSTOM_CONSTRUCTOR_BEGIN(Class_) \
    SAPFIRE_ZERO_CONSTRUCTOR_BEGIN(Class_)       \
    SAPFIRE_CUTOFF_CONSTRUCTOR_BEGIN(Class_)

#define SAPFIRE_CUSTOM_CONSTRUCTOR_COCOS(Class_, Base_) \
    SAPFIRE_ZERO_CONSTRUCTOR(Class_, Base_)             \
    SAPFIRE_CUTOFF_CONSTRUCTOR_COCOS(Class_, Base_)

#define SAPFIRE_CUSTOM_CONSTRUCTOR_GD(Class_, Base_) \
    SAPFIRE_ZERO_CONSTRUCTOR(Class_, Base_)          \
    SAPFIRE_CUTOFF_CONSTRUCTOR_GD(Class_, Base_)

#define SAPFIRE_CUSTOM_CONSTRUCTOR_CUTOFF(Class_, Base_) \
    SAPFIRE_ZERO_CONSTRUCTOR(Class_, Base_)              \
    SAPFIRE_CUTOFF_CONSTRUCTOR_CUTOFF(Class_, Base_)

#define SAPFIRE_ZERO_CONSTRUCTOR_BEGIN(Class_)                                              \
    Class_(sapfire::ZeroConstructorType, void*) {}                                          \
    Class_(sapfire::ZeroConstructorType, size_t fill) :                                     \
        Class_(sapfire::ZeroConstructor, std::memset(static_cast<void*>(this), 0, fill)) {} \
    Class_(sapfire::ZeroConstructorType) : Class_(sapfire::ZeroConstructor, nullptr) {}

#define SAPFIRE_ZERO_CONSTRUCTOR(Class_, Base_)                                                \
    Class_(sapfire::ZeroConstructorType, size_t fill) : Base_(sapfire::ZeroConstructor, fill) {} \
    Class_(sapfire::ZeroConstructorType) : Base_(sapfire::ZeroConstructor, sizeof(Class_)) {}

#define SAPFIRE_FILL_CONSTRUCTOR(Class_, Offset_)                                          \
    Class_(sapfire::CutoffConstructorType, size_t fill) :                                  \
        Class_(                                                                          \
            sapfire::CutoffConstructor,                                                    \
            std::memset(reinterpret_cast<std::byte*>(this) + Offset_, 0, fill - Offset_) \
        ) {}                                                                             \
    Class_(sapfire::CutoffConstructorType, void*)

#define SAPFIRE_CUTOFF_CONSTRUCTOR_BEGIN(Class_)                      \
    SAPFIRE_MACOS(SAPFIRE_FILL_CONSTRUCTOR(Class_, 0){})                \
    SAPFIRE_IOS(SAPFIRE_FILL_CONSTRUCTOR(Class_, 0){})                  \
    SAPFIRE_WINDOWS(Class_(sapfire::CutoffConstructorType, size_t fill) \
                  : Class_() {})                                    \
    SAPFIRE_ANDROID(SAPFIRE_FILL_CONSTRUCTOR(Class_, 0){})

#define SAPFIRE_CUTOFF_CONSTRUCTOR_COCOS(Class_, Base_)               \
    SAPFIRE_MACOS(Class_(sapfire::CutoffConstructorType, size_t fill)   \
                : Base_(sapfire::CutoffConstructor, fill){})          \
    SAPFIRE_IOS(Class_(sapfire::CutoffConstructorType, size_t fill)     \
              : Base_(sapfire::CutoffConstructor, fill){})            \
    SAPFIRE_WINDOWS(Class_(sapfire::CutoffConstructorType, size_t fill) \
                  : Class_() {})                                    \
    SAPFIRE_ANDROID(Class_(sapfire::CutoffConstructorType, size_t fill)   \
                : Base_(sapfire::CutoffConstructor, fill){})

#define SAPFIRE_CUTOFF_CONSTRUCTOR_GD(Class_, Base_)                  \
    SAPFIRE_WINDOWS(Class_(sapfire::CutoffConstructorType, size_t fill) \
                  : Base_(sapfire::CutoffConstructor, fill){})        \
    SAPFIRE_MACOS(Class_(sapfire::CutoffConstructorType, size_t fill)   \
                : Base_(sapfire::CutoffConstructor, fill){})          \
    SAPFIRE_IOS(Class_(sapfire::CutoffConstructorType, size_t fill)     \
              : Base_(sapfire::CutoffConstructor, fill){})            \
    SAPFIRE_ANDROID(Class_(sapfire::CutoffConstructorType, size_t fill) \
              : Base_(sapfire::CutoffConstructor, fill){})

#define SAPFIRE_CUTOFF_CONSTRUCTOR_CUTOFF(Class_, Base_)                       \
    SAPFIRE_WINDOWS(SAPFIRE_FILL_CONSTRUCTOR(Class_, sizeof(Base_)) : Base_(){}) \
    SAPFIRE_ANDROID(Class_(sapfire::CutoffConstructorType, size_t fill)          \
                : Base_(sapfire::CutoffConstructor, fill){})                   \
    SAPFIRE_MACOS(Class_(sapfire::CutoffConstructorType, size_t fill)            \
                : Base_(sapfire::CutoffConstructor, fill){})                   \
    SAPFIRE_IOS(Class_(sapfire::CutoffConstructorType, size_t fill)              \
              : Base_(sapfire::CutoffConstructor, fill){})

#define SAPFIRE_NUMBER_OF_ARGS(...) \
    SAPFIRE_EXPAND(SAPFIRE_NUMBER_OF_ARGS_(__VA_ARGS__, SAPFIRE_NUMBER_SEQUENCE(), ))
#define SAPFIRE_NUMBER_OF_ARGS_(...) SAPFIRE_EXPAND(SAPFIRE_NUMBER_OF_ARGS_N(__VA_ARGS__))
#define SAPFIRE_NUMBER_OF_ARGS_N( \
    _1,                         \
    _2,                         \
    _3,                         \
    _4,                         \
    _5,                         \
    _6,                         \
    _7,                         \
    _8,                         \
    _9,                         \
    _10,                        \
    _11,                        \
    _12,                        \
    _13,                        \
    _14,                        \
    _15,                        \
    _16,                        \
    _17,                        \
    _18,                        \
    _19,                        \
    _20,                        \
    _21,                        \
    _22,                        \
    _23,                        \
    _24,                        \
    _25,                        \
    _26,                        \
    _27,                        \
    _28,                        \
    _29,                        \
    _30,                        \
    _31,                        \
    _32,                        \
    _33,                        \
    _34,                        \
    _35,                        \
    _36,                        \
    _37,                        \
    _38,                        \
    _39,                        \
    _40,                        \
    _41,                        \
    _42,                        \
    _43,                        \
    _44,                        \
    _45,                        \
    _46,                        \
    _47,                        \
    _48,                        \
    _49,                        \
    _50,                        \
    _51,                        \
    _52,                        \
    _53,                        \
    _54,                        \
    _55,                        \
    _56,                        \
    _57,                        \
    _58,                        \
    _59,                        \
    _60,                        \
    _61,                        \
    _62,                        \
    _63,                        \
    N,                          \
    ...                         \
)                               \
    N
#define SAPFIRE_NUMBER_SEQUENCE()                                                                 \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, \
        40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, \
        18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define $execute                                                                                  \
    template <class>                                                                              \
    void SAPFIRE_CONCAT(sapfireExecFunction, __LINE__)();                                             \
    namespace {                                                                                   \
        struct SAPFIRE_CONCAT(ExecFuncUnique, __LINE__) {};                                         \
    }                                                                                             \
    static inline auto SAPFIRE_CONCAT(Exec, __LINE__) =                                             \
        (SAPFIRE_CONCAT(sapfireExecFunction, __LINE__) < SAPFIRE_CONCAT(ExecFuncUnique, __LINE__) > (), \
         0);                                                                                      \
    template <class>                                                                              \
    void SAPFIRE_CONCAT(sapfireExecFunction, __LINE__)()

#define SAPFIRE_FORWARD_COMPAT_DISABLE_HOOKS_INNER(message) \
    if (Loader::get()->isForwardCompatMode()) {           \
        if (strlen(message)) {                            \
            log::warn("[Forward Compat] " message);       \
        }                                                 \
        for (const auto& [_, hook] : self.m_hooks) {      \
            hook->setAutoEnable(false);                   \
        }                                                 \
    }
#define SAPFIRE_FORWARD_COMPAT_ENABLE_HOOKS_INNER(message)  \
    if (!Loader::get()->isForwardCompatMode()) {          \
        if (strlen(message)) {                            \
            log::warn("[Forward Compat] " message);       \
        }                                                 \
        for (const auto& [_, hook] : self.m_hooks) {      \
            hook->setAutoEnable(false);                   \
        }                                                 \
    }
#define SAPFIRE_FORWARD_COMPAT_DISABLE_HOOKS(message)       \
    static void onModify(const auto& self) {              \
        SAPFIRE_FORWARD_COMPAT_DISABLE_HOOKS_INNER(message) \
    }
#define SAPFIRE_FORWARD_COMPAT_ENABLE_HOOKS(message)        \
    static void onModify(const auto& self) {              \
        SAPFIRE_FORWARD_COMPAT_ENABLE_HOOKS_INNER(message)  \
    }

// #define SAPFIRE_NEST1(macro, begin)           \
// macro(SAPFIRE_CONCAT(begin, 0)),                        \
// macro(SAPFIRE_CONCAT(begin, 1)),                        \
// macro(SAPFIRE_CONCAT(begin, 2)),                        \
// macro(SAPFIRE_CONCAT(begin, 3)),                        \
// macro(SAPFIRE_CONCAT(begin, 4)),                        \
// macro(SAPFIRE_CONCAT(begin, 5)),                        \
// macro(SAPFIRE_CONCAT(begin, 6)),                        \
// macro(SAPFIRE_CONCAT(begin, 7)),                        \
// macro(SAPFIRE_CONCAT(begin, 8)),                        \
// macro(SAPFIRE_CONCAT(begin, 9)),                        \
// macro(SAPFIRE_CONCAT(begin, a)),                        \
// macro(SAPFIRE_CONCAT(begin, b)),                        \
// macro(SAPFIRE_CONCAT(begin, c)),                        \
// macro(SAPFIRE_CONCAT(begin, d)),                        \
// macro(SAPFIRE_CONCAT(begin, e)),                        \
// macro(SAPFIRE_CONCAT(begin, f))

// #define SAPFIRE_NEST2(macro, begin)           \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 0)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 1)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 2)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 3)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 4)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 5)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 6)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 7)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 8)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, 9)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, a)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, b)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, c)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, d)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, e)), \
// SAPFIRE_NEST1(macro, SAPFIRE_CONCAT(begin, f))

// #define SAPFIRE_NEST3(macro, begin)           \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 0)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 1)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 2)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 3)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 4)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 5)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 6)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 7)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 8)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, 9)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, a)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, b)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, c)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, d)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, e)), \
// SAPFIRE_NEST2(macro, SAPFIRE_CONCAT(begin, f))

// #define SAPFIRE_NEST4(macro, begin)           \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 0)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 1)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 2)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 3)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 4)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 5)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 6)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 7)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 8)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, 9)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, a)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, b)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, c)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, d)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, e)), \
// SAPFIRE_NEST3(macro, SAPFIRE_CONCAT(begin, f))

// #define SAPFIRE_ENUM_OFFSETS_DEFINE(hex) SAPFIRE_CONCAT($, hex)
// #define SAPFIRE_ENUM_OFFSETS_SET() SAPFIRE_NEST4(SAPFIRE_ENUM_OFFSETS_DEFINE, 0x)

// enum class PrinterOffsets {
//    SAPFIRE_ENUM_OFFSETS_SET()
// };
