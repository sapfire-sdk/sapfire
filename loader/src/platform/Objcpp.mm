// Only a single objc++ file is used because since pch doesnt work, each file adds a lot to the compile times
#include <Sapfire/DefaultInclude.hpp>

using namespace sapfire::prelude;

#if defined(SAPFIRE_IS_MACOS)

#include "mac/LoaderImpl.mm"
#include "mac/main.mm"
#include "mac/crashlog.mm"
#include "mac/FileWatcher.mm"
#include "mac/util.mm"

#elif defined(SAPFIRE_IS_IOS)

#include "ios/FileWatcher.mm"
#include "ios/util.mm"

#endif