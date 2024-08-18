/**
 * Adapted from https://gist.github.com/altalk23/29b97969e9f0624f783b673f6c1cd279
 */

#include <Sapfire/DefaultInclude.hpp>
#include <Sapfire/utils/addresser.hpp>
#include <cstdlib>
#include <stddef.h>

#define SAPFIRE_ADDRESSER_NEST1(macro, begin)                                                      \
    macro(SAPFIRE_CONCAT(begin, 0)), macro(SAPFIRE_CONCAT(begin, 1)), macro(SAPFIRE_CONCAT(begin, 2)), \
        macro(SAPFIRE_CONCAT(begin, 3)), macro(SAPFIRE_CONCAT(begin, 4)),                            \
        macro(SAPFIRE_CONCAT(begin, 5)), macro(SAPFIRE_CONCAT(begin, 6)),                            \
        macro(SAPFIRE_CONCAT(begin, 7)), macro(SAPFIRE_CONCAT(begin, 8)),                            \
        macro(SAPFIRE_CONCAT(begin, 9)), macro(SAPFIRE_CONCAT(begin, a)),                            \
        macro(SAPFIRE_CONCAT(begin, b)), macro(SAPFIRE_CONCAT(begin, c)),                            \
        macro(SAPFIRE_CONCAT(begin, d)), macro(SAPFIRE_CONCAT(begin, e)), macro(SAPFIRE_CONCAT(begin, f))

#define SAPFIRE_ADDRESSER_NEST2(macro, begin)                   \
    SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 0)),     \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 1)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 2)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 3)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 4)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 5)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 6)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 7)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 8)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, 9)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, a)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, b)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, c)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, d)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, e)), \
        SAPFIRE_ADDRESSER_NEST1(macro, SAPFIRE_CONCAT(begin, f))

#define SAPFIRE_ADDRESSER_THUNK0_DEFINE(hex) (intptr_t) & f<hex * sizeof(intptr_t)>
#define SAPFIRE_ADDRESSER_TABLE_DEFINE(hex) (intptr_t) & ThunkTable::table

#define SAPFIRE_ADDRESSER_THUNK0_SET() SAPFIRE_ADDRESSER_NEST2(SAPFIRE_ADDRESSER_THUNK0_DEFINE, 0x)
#define SAPFIRE_ADDRESSER_TABLE_SET() SAPFIRE_ADDRESSER_NEST2(SAPFIRE_ADDRESSER_TABLE_DEFINE, 0x)

using namespace sapfire::addresser;

namespace {
    template <ptrdiff_t index>
    SAPFIRE_HIDDEN ptrdiff_t f() {
        return index;
    }

    using thunk0_table_t = intptr_t[0x100];
    using table_table_t = intptr_t[0x100];

    struct SAPFIRE_HIDDEN ThunkTable {
        static inline thunk0_table_t table = {SAPFIRE_ADDRESSER_THUNK0_SET()};
    };

    class SAPFIRE_HIDDEN TableTable {
        friend class sapfire::addresser::Addresser;

        static inline table_table_t table = {SAPFIRE_ADDRESSER_TABLE_SET()};
    };
}

Addresser::MultipleInheritance* Addresser::instance() {
    return reinterpret_cast<Addresser::MultipleInheritance*>(&TableTable::table);
}

#ifdef SAPFIRE_IS_WINDOWS
#include <delayimp.h>
extern "C" FARPROC WINAPI __delayLoadHelper2(PCImgDelayDescr pidd, FARPROC* ppfnIATEntry); // NOLINT(*-reserved-identifier)

FARPROC WINAPI delayLoadHook(unsigned dliNotify, PDelayLoadInfo pdli) {
    switch (dliNotify) {
        case dliFailLoadLib:
        case dliFailGetProc:
            // incase the delayload helper fails at all (missing symbol, or library entirely),
            // return -1, so we can more easily handle it below
            return (FARPROC)(-1);
        default:
            return NULL;
    }
}

extern "C" const PfnDliHook __pfnDliFailureHook2 = delayLoadHook;
#endif

intptr_t Addresser::followThunkFunction(intptr_t address) {
#ifdef SAPFIRE_IS_WINDOWS32
    // if theres a jmp at the start
    if (address && *reinterpret_cast<uint8_t*>(address) == 0xE9) {
        auto relative = *reinterpret_cast<uint32_t*>(address + 1);
        auto newAddress = address + relative + 5;
        // and if that jmp leads to a jmp dword ptr, only then follow it,
        // because otherwise its just a hook.
        // For some reason this [jmp -> jmp dword ptr] chain happens with a few cocos functions,
        // but not all. For example: cocos2d::ZipUtils::decompressString2
        if (*reinterpret_cast<uint8_t*>(newAddress) == 0xFF && *reinterpret_cast<uint8_t*>(newAddress + 1) == 0x25) {
            address = newAddress;
        }
    }

    // check if first instruction is a jmp dword ptr [....], i.e. if the func is a thunk
    if (address && *reinterpret_cast<uint8_t*>(address) == 0xFF && *reinterpret_cast<uint8_t*>(address + 1) == 0x25) {
        // read where the jmp reads from
        address = *reinterpret_cast<uint32_t*>(address + 2);
        // that then contains the actual address of the func
        address = *reinterpret_cast<uintptr_t*>(address);
    }

    // if it starts with mov eax,..., it's a delay loaded func
    if (address && *reinterpret_cast<uint8_t*>(address) == 0xB8) {
        // follow the jmp to the tailMerge func and grab the ImgDelayDescr pointer from there
        // do it this way instead of grabbing it from the NT header ourselves because
        // we don't know the dll name
        auto iddAddrRel = *reinterpret_cast<uint32_t*>(address + 6);
        auto iddAddr = address + 5 + iddAddrRel + 5;
        iddAddr = *reinterpret_cast<uintptr_t*>(iddAddr + 4);
        auto idd = reinterpret_cast<PCImgDelayDescr>(iddAddr);

        // read where the mov reads from, this is a ptr to the import
        address = *reinterpret_cast<uint32_t*>(address + 1);
        auto imp = reinterpret_cast<FARPROC*>(address);

        // get the address of the function, loading the library if needed
        address = reinterpret_cast<intptr_t>(__delayLoadHelper2(idd, imp));

        // if the helper failed, it will return -1, so we can handle it here
        if (address == -1) {
            address = 0;
        }
    }
#endif
#ifdef SAPFIRE_IS_WINDOWS64
    static constexpr auto checkByteSequence = [](uintptr_t address, const std::initializer_list<uint8_t>& bytes) {
        for (auto byte : bytes) {
            if (*reinterpret_cast<uint8_t*>(address++) != byte) {
                return false;
            }
        }
        return true;
    };

    // check if first instruction is a jmp qword ptr [rip + ...], i.e. if the func is a thunk
    // FF 25 xxxxxxxx
    if (address && checkByteSequence(address, {0xFF, 0x25})) {
        const auto offset = *reinterpret_cast<int32_t*>(address + 2);
        // rip is at address + 6 (size of the instruction)
        address = *reinterpret_cast<uintptr_t*>(address + 6 + offset);
    }

    // if it starts with lea eax,..., it's a delay loaded func
    // 48 8D 05 xxxxxxxx
    if (address && checkByteSequence(address, {0x48, 0x8d, 0x05})) {
        // follow the jmp to the tailMerge func and grab the ImgDelayDescr pointer from there
        // do it this way instead of grabbing it from the NT header ourselves because
        // we don't know the dll name
        auto leaAddress = address + 7 + *reinterpret_cast<int32_t*>(address + 3);

        auto jmpOffset = *reinterpret_cast<int32_t*>(address + 7 + 1);
        auto tailMergeAddr = address + 7 + jmpOffset + 5;
        // inside of the tail merge, try to find the lea rcx, [rip + ...]
        for (uintptr_t leaOffset = 10; leaOffset < 100; ++leaOffset) {
            auto leaAddr = tailMergeAddr + leaOffset;
            if (checkByteSequence(leaAddr, {0x48, 0x8d, 0x0d})) {
                auto offset = *reinterpret_cast<int32_t*>(leaAddr + 3);
                auto did = reinterpret_cast<PCImgDelayDescr>(leaAddr + 7 + offset);
                address = reinterpret_cast<intptr_t>(__delayLoadHelper2(did, reinterpret_cast<FARPROC*>(leaAddress)));

                if (address == -1) {
                    address = 0;
                }
                break;
            }
        }
    }

    // if theres a jmp at the start
    if (address && *reinterpret_cast<uint8_t*>(address) == 0xE9) {
        auto relative = *reinterpret_cast<uint32_t*>(address + 1);
        auto newAddress = address + relative + 5;
        address = newAddress;
    }
#endif
    return address;
}
