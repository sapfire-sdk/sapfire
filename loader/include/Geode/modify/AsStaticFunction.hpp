#pragma once
#include "../utils/addresser.hpp"
#include "Traits.hpp"
#include "../loader/Log.hpp"

namespace sapfire::modifier {
/**
 * A helper struct that generates a static function that calls the given function.
 */
#define SAPFIRE_AS_STATIC_FUNCTION(FunctionName_)                                                   \
    template <class Class2, class FunctionType>                                                   \
    struct AsStaticFunction_##FunctionName_ {                                                     \
        template <class FunctionType2>                                                            \
        struct Impl {};                                                                           \
        template <class Return, class... Params>                                                  \
        struct Impl<Return (*)(Params...)> {                                                      \
            static Return SAPFIRE_CDECL_CALL function(Params... params) {                           \
                return Class2::FunctionName_(params...);                                          \
            }                                                                                     \
        };                                                                                        \
        template <class Return, class Class, class... Params>                                     \
        struct Impl<Return (Class::*)(Params...)> {                                               \
            static Return SAPFIRE_CDECL_CALL function(Class* self, Params... params) {              \
                auto self2 = addresser::rthunkAdjust(                                             \
                    Resolve<Params...>::func(&Class2::FunctionName_), self                        \
                );                                                                                \
                return self2->Class2::FunctionName_(params...);                                   \
            }                                                                                     \
        };                                                                                        \
        template <class Return, class Class, class... Params>                                     \
        struct Impl<Return (Class::*)(Params...) const> {                                         \
            static Return SAPFIRE_CDECL_CALL function(Class const* self, Params... params) {        \
                auto self2 = addresser::rthunkAdjust(                                             \
                    Resolve<Params...>::func(&Class2::FunctionName_), self                        \
                );                                                                                \
                return self2->Class2::FunctionName_(params...);                                   \
            }                                                                                     \
        };                                                                                        \
        static constexpr auto value = &Impl<FunctionType>::function;                              \
    };

    SAPFIRE_AS_STATIC_FUNCTION(constructor)
    SAPFIRE_AS_STATIC_FUNCTION(destructor)


#define SAPFIRE_CONCEPT_FUNCTION_CHECK(FunctionName_)                                              \
    template <class Class, class... Args>                                                        \
    concept FunctionExists_##FunctionName_ = requires(Class* self, Args... args) {               \
        self->FunctionName_(args...);                                                            \
    };
}
