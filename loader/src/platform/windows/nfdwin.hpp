/*
  Native File Dialog
  http://www.frogtoss.com/labs
 */

/**
 * Copied from https://github.com/mlabbe/nativefiledialog
 * Modified to be modern Sapfire-fitting C++
 */

#include <Sapfire/DefaultInclude.hpp>

#ifdef __MINGW32__
// Explicitly setting NTDDI version, this is necessary for the MinGW compiler
#define NTDDI_VERSION NTDDI_VISTA
#define _WIN32_WINNT _WIN32_WINNT_VISTA
#endif

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

/* only locally define UNICODE in this compilation unit */
#ifndef UNICODE
#define UNICODE
#endif

#include <wchar.h>
#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <shobjidl.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NFD_MAX_STRLEN 256
#define _NFD_UNUSED(x) ((void)x)

#define NFD_UTF8_BOM "\xEF\xBB\xBF"

#include <stddef.h>
#include <Sapfire/utils/Result.hpp>
#include <Sapfire/utils/file.hpp>

using namespace sapfire::prelude;

enum class NFDMode {
    OpenFile,
    OpenFiles,
    SaveFile,
    OpenFolder,
};

Result<> nfdPick(
    NFDMode mode,
    file::FilePickOptions const& options,
    void* result
);
