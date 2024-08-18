cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded)
cmake_policy(SET CMP0141 NEW)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries static" FORCE)

# Docs flags
if (SAPFIRE_BUILDING_DOCS)
	set(SAPFIRE_DISABLE_CLI_CALLS ON)
	set(SAPFIRE_DISABLE_PRECOMPILED_HEADERS ON)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(SAPFIRE_DONT_BUILD_TEST_MODS ON)
endif()

option(SAPFIRE_USE_BREAKPAD "Enables the use of the Breakpad library for crash dumps." ON)

# Read version
file(READ VERSION SAPFIRE_VERSION)
string(STRIP "${SAPFIRE_VERSION}" SAPFIRE_VERSION)
set(SAPFIRE_VERSION "${SAPFIRE_VERSION}" CACHE INTERNAL "Sapfire version")
set(SAPFIRE_VERSION_FULL "${SAPFIRE_VERSION}" CACHE INTERNAL "Sapfire version full")

# Check if version has a tag like v1.0.0-alpha
string(FIND ${SAPFIRE_VERSION} "-" SAPFIRE_VERSION_HAS_TAG)
if (NOT ${SAPFIRE_VERSION_HAS_TAG} EQUAL "-1")
	string(REGEX MATCH "[a-z]+(\.[0-9]+)?$" SAPFIRE_VERSION_TAG ${SAPFIRE_VERSION})
	string(SUBSTRING "${SAPFIRE_VERSION}" 0 ${SAPFIRE_VERSION_HAS_TAG} SAPFIRE_VERSION)
	string(FIND ${SAPFIRE_VERSION_TAG} "." SAPFIRE_VERSION_TAG_HAS_NUMBER)

	# Extract tag type and number from tag
	if (NOT ${SAPFIRE_VERSION_TAG_HAS_NUMBER} EQUAL "-1")
		string(SUBSTRING "${SAPFIRE_VERSION_TAG}" 0 ${SAPFIRE_VERSION_TAG_HAS_NUMBER} SAPFIRE_VERSION_TAG_TYPE)
		math(EXPR SAPFIRE_VERSION_TAG_HAS_NUMBER "${SAPFIRE_VERSION_TAG_HAS_NUMBER} + 1")
		string(SUBSTRING "${SAPFIRE_VERSION_TAG}" ${SAPFIRE_VERSION_TAG_HAS_NUMBER} -1 SAPFIRE_VERSION_TAG_NUMBER)
	else()
		set(SAPFIRE_VERSION_TAG_TYPE "${SAPFIRE_VERSION_TAG}")
		set(SAPFIRE_VERSION_TAG_NUMBER "")
	endif()

	# Capitalize first letter of tag type
	string(SUBSTRING ${SAPFIRE_VERSION_TAG_TYPE} 0 1 FIRST_LETTER)
	string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
	string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" SAPFIRE_VERSION_TAG_TYPE "${SAPFIRE_VERSION_TAG_TYPE}")

	message(STATUS "Version: ${SAPFIRE_VERSION}, tag: ${SAPFIRE_VERSION_TAG} (type: ${SAPFIRE_VERSION_TAG_TYPE}, number: ${SAPFIRE_VERSION_TAG_NUMBER})")
else()
	set(SAPFIRE_VERSION_TAG "")
	set(SAPFIRE_VERSION_TAG_TYPE "")
	set(SAPFIRE_VERSION_TAG_NUMBER "")

	message(STATUS "Version: ${SAPFIRE_VERSION}")
endif()

include(cmake/CPM.cmake)

if (NOT DEFINED CPM_SOURCE_CACHE)
	message(NOTICE "Not using CPM cache (CPM_SOURCE_CACHE). "
		"It is recommended to use it to improve configure times.")
endif()

# Allow users to have their own copy of bindings that can be overwritten with a CMake option.
# If the option is not provided, by default just clone bindings with CPM and use that
if (DEFINED ENV{SAPFIRE_BINDINGS_REPO_PATH})
	set(temp $ENV{SAPFIRE_BINDINGS_REPO_PATH})
	# this is so stupid i hate windows paths
	string(REPLACE "\\" "/" SAPFIRE_BINDINGS_REPO_PATH ${temp})
endif()

if (NOT SAPFIRE_BINDINGS_REPO_PATH)
	message(STATUS
		"No override path for bindings provided, using CPM to clone default. "
		"If you would like to use a separate clone of the bindings repo "
		"(for example in order to be able to efficiently change and "
		"contribute new bindings) then set SAPFIRE_BINDINGS_REPO_PATH to where you have "
		"cloned the repository (system environment variables are supported)."
	)
	CPMAddPackage(NAME "bindings"
		GITHUB_REPOSITORY "sapfire-sdk/bindings"
		GIT_TAG "main"
		DOWNLOAD_ONLY YES
		NO_CACHE YES
	)
	set(SAPFIRE_BINDINGS_REPO_PATH ${bindings_SOURCE_DIR})
else()
	message(STATUS "Using ${SAPFIRE_BINDINGS_REPO_PATH} for bindings repo")
endif()

project(sapfire-sdk VERSION ${SAPFIRE_VERSION} LANGUAGES CXX C)

if (NOT SAPFIRE_DONT_USE_CCACHE)
	message(STATUS "Looking for ccache/sccache")
	if (DEFINED CMAKE_C_COMPILER_LAUNCHER OR DEFINED CMAKE_CXX_COMPILER_LAUNCHER)
		message(STATUS "Looking for ccache/sccache - detecting variant")
		if (DEFINED CMAKE_C_COMPILER_LAUNCHER AND DEFINED CMAKE_CXX_COMPILER_LAUNCHER AND
			CMAKE_C_COMPILER_LAUNCHER STREQUAL CMAKE_CXX_COMPILER_LAUNCHER)
			if (CMAKE_CXX_COMPILER_LAUNCHER MATCHES "sccache(|.exe)$")
				message(STATUS "Looking for ccache/sccache - detected sccache")
				set(SAPFIRE_CCACHE_VARIANT "sccache")
			elseif(CMAKE_CXX_COMPILER_LAUNCHER MATCHES "ccache(|.exe)$")
				message(STATUS "Looking for ccache/sccache - detected ccache")
				set(SAPFIRE_CCACHE_VARIANT "ccache")
			else()
				message(STATUS "Looking for ccache/sccache - none (custom compiler launcher: {CMAKE_C_COMPILER_LAUNCHER}, {CMAKE_CXX_COMPILER_LAUNCHER})")
			endif()
		else()
			message(STATUS "Looking for ccache/sccache - skipped (custom compiler launcher: {CMAKE_C_COMPILER_LAUNCHER}, {CMAKE_CXX_COMPILER_LAUNCHER})")
		endif()
	elseif (CMAKE_GENERATOR MATCHES "Visual Studio")
		message(STATUS "Looking for ccache/sccache - skipped (Visual Studio generator)")
	else()
		find_program(SAPFIRE_CCACHE NAMES sccache ccache)
		if (SAPFIRE_CCACHE)
			if (${SAPFIRE_CCACHE} MATCHES "sccache(|.exe)$")
				set(SAPFIRE_CCACHE_VARIANT "sccache")
			else()
				set(SAPFIRE_CCACHE_VARIANT "ccache")
			endif()
			message(STATUS "Looking for ccache/sccache - found ${SAPFIRE_CCACHE_VARIANT}")
			message(NOTICE "Compiler launcher not set but ccache/sccache was found. "
				"Setting compiler launcher to that")
			set(CMAKE_C_COMPILER_LAUNCHER ${SAPFIRE_CCACHE})
			set(CMAKE_CXX_COMPILER_LAUNCHER ${SAPFIRE_CCACHE})
		else()
			message(STATUS "Looking for ccache/sccache - not found")
		endif()
		unset(SAPFIRE_CCACHE)
	endif()
else()
	message(STATUS "Not looking for ccache because SAPFIRE_DONT_USE_CCACHE was ON")
endif()

if (DEFINED SAPFIRE_CCACHE_VARIANT)
	if (NOT DEFINED SAPFIRE_DISABLE_PRECOMPILED_HEADERS)
		if (${SAPFIRE_CCACHE_VARIANT} STREQUAL "sccache" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			message(NOTICE "Using ${SAPFIRE_CCACHE_VARIANT} with ${CMAKE_CXX_COMPILER_ID}, PCH will be enabled.")
			set(SAPFIRE_DISABLE_PRECOMPILED_HEADERS OFF)
		else()
			message(NOTICE "Using ${SAPFIRE_CCACHE_VARIANT} with ${CMAKE_CXX_COMPILER_ID}, PCH will be disabled.")
			set(SAPFIRE_DISABLE_PRECOMPILED_HEADERS ON)
		endif()
	endif()
else()
	message(NOTICE "Not using a caching compiler (ccache/sccache). "
		"It is recommended to install one to improve build times.")
	message(NOTICE "We recommend sccache, check its README for installation instructions, "
		"normally you can just use your usual package manager (e.g. 'scoop install sccache').")
	if (NOT DEFINED SAPFIRE_DISABLE_PRECOMPILED_HEADERS)
		message(NOTICE "Because of this, PCH will be enabled.")
		set(SAPFIRE_DISABLE_PRECOMPILED_HEADERS OFF)
	endif()
endif()

if (NOT DEFINED SAPFIRE_DISABLE_PRECOMPILED_HEADERS)
	set(SAPFIRE_DISABLE_PRECOMPILED_HEADERS OFF)
endif()

if (SAPFIRE_DISABLE_PRECOMPILED_HEADERS)
	message(STATUS "Pre-compiled headers - OFF")
else()
	message(STATUS "Pre-compiled headers - ON")
endif()

add_library(${PROJECT_NAME} INTERFACE)

# Rerun CMake on VERSION file change
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CONFIGURE_DEPENDS VERSION)

target_compile_definitions(${PROJECT_NAME} INTERFACE -DPROJECT_NAME=${CMAKE_PROJECT_NAME})

set(SAPFIRE_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(SAPFIRE_LOADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/loader)
set(SAPFIRE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/Platform.cmake)
include(cmake/SapfireFile.cmake)

if (NOT DEFINED SAPFIRE_GD_VERSION)
	set(SAPFIRE_GD_VERSION 2.206)
	set(SAPFIRE_COMP_GD_VERSION 22060)
endif()

target_compile_definitions(${PROJECT_NAME} INTERFACE SAPFIRE_GD_VERSION=${SAPFIRE_GD_VERSION} SAPFIRE_COMP_GD_VERSION=${SAPFIRE_COMP_GD_VERSION})

if (WIN32)
	# This allows you to compile in debug mode
	# add_compile_definitions(_HAS_ITERATOR_DEBUGGING=0)
	add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
	target_compile_definitions(${PROJECT_NAME} INTERFACE _HAS_ITERATOR_DEBUGGING=0)
	
	target_link_libraries(${PROJECT_NAME} INTERFACE delayimp ws2_32)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
		CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
		# target x86 on windows with clang
		target_compile_options(${PROJECT_NAME} INTERFACE --target=x86_64-pc-windows-msvc)
		target_link_options(${PROJECT_NAME} INTERFACE --target=x86_64-pc-windows-msvc)
		add_compile_options(--target=x86_64-pc-windows-msvc)
		add_link_options(--target=x86_64-pc-windows-msvc)
	endif()
endif()

# if (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# 	set_property(TARGET ${PROJECT_NAME} PROPERTY LINKER_TYPE LLD)
# 	target_link_options(${PROJECT_NAME} INTERFACE -fuse-ld=lld)
# endif ()

# We do not support anything below SDK 23
if (ANDROID)
	string(REGEX MATCH "[0-9]+" ANDROID_PLATFORM_NUMBER "${ANDROID_PLATFORM}")
	if (ANDROID_PLATFORM_NUMBER LESS 23)
		message(FATAL_ERROR "Specified target Android SDK version is too low (${ANDROID_PLATFORM_NUMBER}), must be 23 or higher")
	endif()
endif()

set(MAT_JSON_AS_INTERFACE ON)
CPMAddPackage("gh:sapfire-sdk/json#cda9807")
CPMAddPackage("gh:fmtlib/fmt#10.2.1")

target_compile_definitions(${PROJECT_NAME} INTERFACE MAT_JSON_DYNAMIC=1)

# this is needed for cross compilation on linux,
# since fmtlib will fail to compile otherwise
if (SAPFIRE_DISABLE_FMT_CONSTEVAL)
	message(VERBOSE "Disabling FMT_CONSTEVAL")
	target_compile_definitions(fmt PUBLIC -DFMT_CONSTEVAL=)
endif()

# Tulip hook (hooking)
if (PROJECT_IS_TOP_LEVEL AND NOT SAPFIRE_BUILDING_DOCS)
	set(TULIP_LINK_SOURCE ON)
endif()
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)

# Allow users to have their own copy of TulipHook that can be overwritten with a CMake option.
# If the option is not provided, by default just clone TulipHook with CPM and use that
if (DEFINED ENV{SAPFIRE_TULIPHOOK_REPO_PATH})
	set(temp $ENV{SAPFIRE_TULIPHOOK_REPO_PATH})
	# this is so stupid i hate windows paths
	string(REPLACE "\\" "/" SAPFIRE_TULIPHOOK_REPO_PATH ${temp})
endif()

if (DEFINED SAPFIRE_TULIPHOOK_REPO_PATH)
	message(STATUS "Using ${SAPFIRE_TULIPHOOK_REPO_PATH} for TulipHook")
	add_subdirectory(${SAPFIRE_TULIPHOOK_REPO_PATH} ${SAPFIRE_TULIPHOOK_REPO_PATH}/build)
else()
	CPMAddPackage("gh:sapfire-sdk/TulipHook#fbf79b4")
endif()
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-everything SUPPORTS_W_NO_EVERYTHING)
if (SUPPORTS_W_NO_EVERYTHING)
	# Silence warnings from dependencies
	if (TARGET capstone)
		target_compile_options(capstone PRIVATE -Wno-everything)
	endif()
endif()

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp)

add_subdirectory(${SAPFIRE_BINDINGS_REPO_PATH} ${CMAKE_BINARY_DIR}/bindings)

if (NOT SAPFIRE_DISABLE_PRECOMPILED_HEADERS)
	target_precompile_headers(SapfireBindings INTERFACE
		"$<$<COMPILE_LANGUAGE:CXX>:${SAPFIRE_LOADER_PATH}/include/Sapfire/Bindings.hpp>"
	)
endif()

target_include_directories(SapfireBindings PUBLIC
	${SAPFIRE_LOADER_PATH}/include
	${SAPFIRE_LOADER_PATH}/include/Sapfire/cocos/include
	${SAPFIRE_LOADER_PATH}/include/Sapfire/cocos/extensions
	${SAPFIRE_LOADER_PATH}/include/Sapfire/fmod
)
target_link_directories(SapfireBindings PUBLIC ${SAPFIRE_LOADER_PATH}/include/link)
target_link_libraries(SapfireBindings PUBLIC fmt TulipHookInclude mat-json)
target_link_libraries(${PROJECT_NAME} INTERFACE SapfireBindings)

if (NOT EXISTS ${SAPFIRE_BIN_PATH})
	file(MAKE_DIRECTORY ${SAPFIRE_BIN_PATH})
endif()

if (NOT EXISTS ${SAPFIRE_BIN_PATH}/${PROJECT_VERSION} AND EXISTS ${SAPFIRE_BIN_PATH}/nightly/)
	set(SAPFIRE_LINK_NIGHTLY 1)
endif()

if (${SAPFIRE_LINK_NIGHTLY})
	set(SAPFIRE_PLATFORM_BIN_PATH ${SAPFIRE_BIN_PATH}/nightly/${SAPFIRE_PLATFORM_BINARY})
else()
	set(SAPFIRE_PLATFORM_BIN_PATH ${SAPFIRE_BIN_PATH}/${PROJECT_VERSION}/${SAPFIRE_PLATFORM_BINARY})
endif()

if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(loader)
	target_link_libraries(${PROJECT_NAME} INTERFACE sapfire-loader)
	if (SUPPORTS_W_NO_EVERYTHING)
		target_compile_options(sapfire-loader PRIVATE -Wno-inconsistent-missing-override)
	endif()
elseif (EXISTS ${SAPFIRE_PLATFORM_BIN_PATH})
	target_link_libraries(${PROJECT_NAME} INTERFACE "${SAPFIRE_PLATFORM_BIN_PATH}")
	if (NOT SAPFIRE_DISABLE_PRECOMPILED_HEADERS)
		target_precompile_headers(${PROJECT_NAME} INTERFACE
			"$<$<COMPILE_LANGUAGE:CXX>:${SAPFIRE_LOADER_PATH}/include/Sapfire/DefaultInclude.hpp>"
			"$<$<COMPILE_LANGUAGE:CXX>:${SAPFIRE_LOADER_PATH}/include/Sapfire/Sapfire.hpp>"
			# please stop adding modify here its not here because it makes windows compilation take longer than sapfire 1.0 release date
		)
	endif()
else()
	message(FATAL_ERROR
		"No valid loader binary to link to! Install pre-built binaries with `sapfire sdk install-binaries`, "
		"or build Sapfire from source and add `set(SAPFIRE_LINK_NIGHTLY ON)` to your CMakeLists.txt "
		"in the line before calling add_subdirectory for Sapfire."
	)
endif()
