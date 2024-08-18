#include <Sapfire/loader/Loader.hpp>
#include <Sapfire/loader/Mod.hpp>

namespace sapfire {
    /**
     * To bypass the need for cyclic dependencies,
     * this function does the exact same as Mod::get()
     * However, it can be externed, unlike Mod::get()
     * @returns Same thing Mod::get() returns
     */
    Mod* getMod() {
        return Mod::get();
    }
}

SAPFIRE_API void sapfireImplicitEntry() {
    // to make sure the instance is set into the sharedMod<> in load time
    (void)sapfire::getMod();
}

#if defined(_DEBUG) && defined(SAPFIRE_IS_WINDOWS)

// This bypasses any of the heap validation measures that are injected when compiling in Debug.
// Without these, the game will very likely crash when the mod tries to free memory allocated by the game (or another non-debug mod).

static inline void* relallocthrow(size_t size) {
    void* p;
    while ((p = HeapAlloc(GetProcessHeap(), 0, size)) == 0) {
        if (_callnewh(size) == 0) {
            static const std::bad_alloc exc;
            throw exc;
        }
    }

    return p;
}

static inline void relfree(void* block) {
    HeapFree(GetProcessHeap(), 0, block);
}

void* operator new(size_t size) {
    return relallocthrow(size);
}

void* operator new[](size_t size) {
    return relallocthrow(size);
}

void operator delete(void* block) noexcept {
    relfree(block);
}

#endif
