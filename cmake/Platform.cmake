include(cmake/PlatformDetect.cmake)

if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
	set(SAPFIRE_TARGET_PLATFORM SAPFIRE_TARGET_PLATFORM PARENT_SCOPE)
endif()

if (SAPFIRE_TARGET_PLATFORM STREQUAL "iOS")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME iOS
		OSX_SYSROOT ${SAPFIRE_IOS_SDK}
		OSX_ARCHITECTURES arm64
	)

	set(SAPFIRE_OUTPUT_NAME "Sapfire.ios")
	set(SAPFIRE_PLATFORM_BINARY "Sapfire.ios.dylib")
	set(SAPFIRE_MOD_BINARY_SUFFIX ".ios.dylib" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(SAPFIRE_TARGET_PLATFORM_SHORT "ios" PARENT_SCOPE)
	else()
		set(SAPFIRE_TARGET_PLATFORM_SHORT "ios")
	endif()
elseif (SAPFIRE_TARGET_PLATFORM STREQUAL "MacOS")
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		SYSTEM_NAME MacOS
	)

	# for some reason, this value is initialized as an empty string by default. idk why
	if (NOT DEFINED CMAKE_OSX_ARCHITECTURES OR CMAKE_OSX_ARCHITECTURES STREQUAL "")
		set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
	endif()

	# only exists as a global property
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		"-framework Cocoa"
		"-framework OpenGL"
		"-framework SystemConfiguration"
		${SAPFIRE_LOADER_PATH}/include/link/macos/libfmod.dylib
		${SAPFIRE_LOADER_PATH}/include/link/macos/libssl.a
		${SAPFIRE_LOADER_PATH}/include/link/macos/libcrypto.a
		${SAPFIRE_LOADER_PATH}/include/link/macos/libnghttp2.a
		${SAPFIRE_LOADER_PATH}/include/link/macos/libngtcp2.a
		${SAPFIRE_LOADER_PATH}/include/link/macos/libnghttp3.a
		${SAPFIRE_LOADER_PATH}/include/link/macos/libngtcp2_crypto_boringssl.a
		${SAPFIRE_LOADER_PATH}/include/link/macos/libcurl.a
	)

	target_compile_definitions(${PROJECT_NAME} INTERFACE
		-DCommentType=CommentTypeDummy
		-DGL_SILENCE_DEPRECATION
	)

	set(SAPFIRE_OUTPUT_NAME "Sapfire")
	set(SAPFIRE_PLATFORM_BINARY "Sapfire.dylib")
	set(SAPFIRE_MOD_BINARY_SUFFIX ".dylib" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(SAPFIRE_TARGET_PLATFORM_SHORT "mac" PARENT_SCOPE)
	else()
		set(SAPFIRE_TARGET_PLATFORM_SHORT "mac")
	endif()
elseif (SAPFIRE_TARGET_PLATFORM STREQUAL "Win64")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Win64
		GENERATOR_PLATFORM x64
	)

	target_compile_definitions(${PROJECT_NAME} INTERFACE NOMINMAX)

	target_link_libraries(${PROJECT_NAME} INTERFACE 
		${SAPFIRE_LOADER_PATH}/include/link/win64/libcocos2d.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/libExtensions.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/ssl.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/crypto.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/nghttp2.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/ngtcp2.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/nghttp3.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/ngtcp2_crypto_boringssl.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/libcurl.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/glew32.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/gdstring.lib
		${SAPFIRE_LOADER_PATH}/include/link/win64/fmod.lib
		opengl32
	)

	# Windows links against .lib and not .dll
	set(SAPFIRE_OUTPUT_NAME "Sapfire")
	set(SAPFIRE_PLATFORM_BINARY "Sapfire.lib")
	set(SAPFIRE_MOD_BINARY_SUFFIX ".dll" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(SAPFIRE_TARGET_PLATFORM_SHORT "win" PARENT_SCOPE)
	else()
		set(SAPFIRE_TARGET_PLATFORM_SHORT "win")
	endif()
elseif (SAPFIRE_TARGET_PLATFORM STREQUAL "Android32")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Android
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		c
		unwind
		${SAPFIRE_LOADER_PATH}/include/link/android32/libssl.a
		${SAPFIRE_LOADER_PATH}/include/link/android32/libcrypto.a
		${SAPFIRE_LOADER_PATH}/include/link/android32/libnghttp2.a
		${SAPFIRE_LOADER_PATH}/include/link/android32/libngtcp2.a
		${SAPFIRE_LOADER_PATH}/include/link/android32/libnghttp3.a
		${SAPFIRE_LOADER_PATH}/include/link/android32/libngtcp2_crypto_boringssl.a
		${SAPFIRE_LOADER_PATH}/include/link/android32/libcurl.a
		${SAPFIRE_LOADER_PATH}/include/link/android32/libcocos2dcpp.so
		${SAPFIRE_LOADER_PATH}/include/link/android32/libfmod.so
		GLESv2
		log
	)

	set(SAPFIRE_OUTPUT_NAME "Sapfire.android32")
	set(SAPFIRE_PLATFORM_BINARY "Sapfire.android32.so")
	set(SAPFIRE_MOD_BINARY_SUFFIX ".android32.so" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(SAPFIRE_TARGET_PLATFORM_SHORT "android32" PARENT_SCOPE)
	else()
		set(SAPFIRE_TARGET_PLATFORM_SHORT "android32")
	endif()
elseif (SAPFIRE_TARGET_PLATFORM STREQUAL "Android64")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Android
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		c
		unwind
		${SAPFIRE_LOADER_PATH}/include/link/android64/libssl.a
		${SAPFIRE_LOADER_PATH}/include/link/android64/libcrypto.a
		${SAPFIRE_LOADER_PATH}/include/link/android64/libnghttp2.a
		${SAPFIRE_LOADER_PATH}/include/link/android64/libngtcp2.a
		${SAPFIRE_LOADER_PATH}/include/link/android64/libnghttp3.a
		${SAPFIRE_LOADER_PATH}/include/link/android64/libngtcp2_crypto_boringssl.a
		${SAPFIRE_LOADER_PATH}/include/link/android64/libcurl.a
		${SAPFIRE_LOADER_PATH}/include/link/android64/libcocos2dcpp.so
		${SAPFIRE_LOADER_PATH}/include/link/android64/libfmod.so
		GLESv2
		log
	)

	# this should help with fixing exceptions
	set(ANDROID_STL c++_shared)
  # a little desperate
	add_definitions(-DANDROID_STL=c++_shared)

	set(SAPFIRE_OUTPUT_NAME "Sapfire.android64")
	set(SAPFIRE_PLATFORM_BINARY "Sapfire.android64.so")
	set(SAPFIRE_MOD_BINARY_SUFFIX ".android64.so" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(SAPFIRE_TARGET_PLATFORM_SHORT "android64" PARENT_SCOPE)
	else()
		set(SAPFIRE_TARGET_PLATFORM_SHORT "android64")
	endif()
else()
	message(FATAL_ERROR "Unknown platform ${SAPFIRE_TARGET_PLATFORM}")
endif()
