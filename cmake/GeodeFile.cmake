set(SAPFIRE_CLI_MINIMUM_VERSION 1.0.5)

# Find Sapfire CLI
if (NOT DEFINED SAPFIRE_CLI OR SAPFIRE_CLI STREQUAL "SAPFIRE_CLI-NOTFOUND")
    find_program(SAPFIRE_CLI NAMES sapfire.exe sapfire-cli.exe sapfire sapfire-cli PATHS ${CLI_PATH})
endif()

# Check if CLI was found
if (SAPFIRE_CLI STREQUAL "SAPFIRE_CLI-NOTFOUND")
	message(WARNING "Unable to find Sapfire CLI")
else()
    # `sapfire --version` returns `sapfire x.x.x\n` so gotta do some wacky shit
    execute_process(
        COMMAND ${SAPFIRE_CLI} --version
        OUTPUT_VARIABLE SAPFIRE_CLI_VERSION
        COMMAND_ERROR_IS_FATAL ANY
    )
    # Remove trailing newline
    string(STRIP ${SAPFIRE_CLI_VERSION} SAPFIRE_CLI_VERSION)
    # Remove program name
    string(REPLACE "sapfire " "" SAPFIRE_CLI_VERSION ${SAPFIRE_CLI_VERSION})

    # Need at least v1.0.5 (--shut-up arg in sapfire package resources)
    if (${SAPFIRE_CLI_VERSION} VERSION_LESS ${SAPFIRE_CLI_MINIMUM_VERSION})
        message(FATAL_ERROR
            "Found Sapfire CLI: ${SAPFIRE_CLI}, however it is version ${SAPFIRE_CLI_VERSION} "
            "while minimum required is version ${SAPFIRE_CLI_MINIMUM_VERSION}. Please update: "
            "https://github.com/sapfire-sdk/cli/releases/latest"
        )
    endif()

    # Cache version so it's available to other functions
    set(SAPFIRE_CLI_VERSION "${SAPFIRE_CLI_VERSION}" CACHE INTERNAL "SAPFIRE_CLI_VERSION")

    message(STATUS "Found Sapfire CLI: ${SAPFIRE_CLI} (version ${SAPFIRE_CLI_VERSION})")
endif()

# Clear cache of mods being built so mods from earlier 
# configures dont appear on the list
set(SAPFIRE_MODS_BEING_BUILT "" CACHE INTERNAL "SAPFIRE_MODS_BEING_BUILT")

# todo: add EXTERNAL argument that takes a list of external mod ids
# current workaround is to manually append to SAPFIRE_MODS_BEING_BUILT before 
# calling setup_sapfire_mod if the mod depends on external dependencies that 
# aren't being built
function(setup_sapfire_mod proname)
    # Get DONT_INSTALL argument
    set(options DONT_INSTALL)
    set(multiValueArgs EXTERNALS)
    cmake_parse_arguments(SETUP_SAPFIRE_MOD "${options}" "" "${multiValueArgs}" ${ARGN})

    # Link Sapfire to the mod
    target_link_libraries(${proname} sapfire-sdk)

    if (ANDROID)
        if (CMAKE_BUILD_TYPE STREQUAL "Release")
            add_custom_command(
                TARGET "${proname}" POST_BUILD
                DEPENDS "${proname}"
                COMMAND $<$<CONFIG:release>:${CMAKE_STRIP}>
                ARGS -S $<TARGET_FILE:${proname}>
            )
        endif()
    endif()

    if (SAPFIRE_DISABLE_CLI_CALLS)
        message("Skipping setting up sapfire mod ${proname}")
        return()
    endif()

    if(SAPFIRE_CLI STREQUAL "SAPFIRE_CLI-NOTFOUND")
        message(FATAL_ERROR
            "setup_sapfire_mod called, but Sapfire CLI was not found - "
            "Please install CLI: https://docs.geode-sdk.org/"
        )
        return()
    endif()

    # what is this for
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mod.json ${CMAKE_CURRENT_BINARY_DIR}/what.txt)
    set_target_properties(${proname} PROPERTIES CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mod.json)

    message(STATUS "Setting up ${proname}")

    # Read mod.json
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/mod.json" MOD_JSON)
    string(JSON MOD_ID GET "${MOD_JSON}" "id")
    string(JSON MOD_VERSION GET "${MOD_JSON}" "version")
    string(JSON TARGET_SAPFIRE_VERSION GET "${MOD_JSON}" "sapfire")
    string(JSON MOD_HAS_API ERROR_VARIABLE MOD_DOESNT_HAVE_API GET "${MOD_JSON}" "api")
    string(JSON MOD_HAS_DEPS ERROR_VARIABLE MOD_DOESNT_HAVE_DEPS GET "${MOD_JSON}" "dependencies")

    if ("${TARGET_SAPFIRE_VERSION}" STREQUAL "${SAPFIRE_VERSION_FULL}")
        message(STATUS "Mod ${MOD_ID} is compiling for Sapfire version ${SAPFIRE_VERSION_FULL}")
    else()
        message(FATAL_ERROR
            "Mod ${MOD_ID} is made for Sapfire version ${TARGET_SAPFIRE_VERSION} but you have ${SAPFIRE_VERSION_FULL} SDK installed. Please change the Sapfire version in your mod.json. "
        )
    endif()

    target_compile_definitions(${proname} PRIVATE SAPFIRE_MOD_ID="${MOD_ID}")

    # Add this mod to the list of known externals mods
    list(APPEND SAPFIRE_MODS_BEING_BUILT "${MOD_ID}:${MOD_VERSION}")
    # Ensure that the list of mods being built is global (persists between setup_sapfire_mod calls)
    set(SAPFIRE_MODS_BEING_BUILT ${SAPFIRE_MODS_BEING_BUILT} CACHE INTERNAL "SAPFIRE_MODS_BEING_BUILT")

    # Add function arg externals to the list but don't cache that
    if (SETUP_SAPFIRE_MOD_EXTERNALS)
        list(APPEND SAPFIRE_MODS_BEING_BUILT ${SETUP_SAPFIRE_MOD_EXTERNALS})
    endif()

    # For CLI >=v2.4.0, there's an option to disable updating index because 
    # Github Actions on Mac just returns 403 for no reason
    if (SAPFIRE_DONT_UPDATE_INDEX AND (${SAPFIRE_CLI_VERSION} VERSION_GREATER_EQUAL "2.4.0"))
        set(DONT_UPDATE_INDEX_ARG "--dont-update-index")
    else()
        set(DONT_UPDATE_INDEX_ARG "")
    endif()

    # Check dependencies using CLI
    if (${SAPFIRE_CLI_VERSION} VERSION_GREATER_EQUAL "3.2.0")
        execute_process(
            COMMAND ${SAPFIRE_CLI} project check ${CMAKE_CURRENT_BINARY_DIR}
                --externals ${SAPFIRE_MODS_BEING_BUILT} ${DONT_UPDATE_INDEX_ARG}
                --platform ${SAPFIRE_TARGET_PLATFORM_SHORT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY
        )
    elseif (${SAPFIRE_CLI_VERSION} VERSION_GREATER_EQUAL "2.0.0")
        message(WARNING "If you use platform-specific dependencies, upgrade Sapfire CLI to version 3.2.0 or greater!")
        execute_process(
            COMMAND ${SAPFIRE_CLI} project check ${CMAKE_CURRENT_BINARY_DIR}
                --externals ${SAPFIRE_MODS_BEING_BUILT} ${DONT_UPDATE_INDEX_ARG}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMAND_ERROR_IS_FATAL ANY
        )
    elseif (${SAPFIRE_CLI_VERSION} VERSION_GREATER_EQUAL "1.4.0")
        execute_process(
            COMMAND ${SAPFIRE_CLI} package setup ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
                --externals ${SAPFIRE_MODS_BEING_BUILT}
            COMMAND_ERROR_IS_FATAL ANY
        )
    elseif (MOD_HAS_DEPS)
        message(FATAL_ERROR
            "CLI is version ${SAPFIRE_CLI_VERSION}, but using dependencies "
            "requires at least 1.4.0 - please update your CLI"
        )
    endif()
    
    # Check if --install should be passed
    if (SETUP_SAPFIRE_MOD_DONT_INSTALL OR SAPFIRE_DONT_INSTALL_MODS)
        message(STATUS "Skipping installing ${proname}")
        set(INSTALL_ARG "")
    else()
        set(INSTALL_ARG "--install")
    endif()

    # The lib binary should be passed only if some headers were provided
    if (MOD_HAS_API)
        message(STATUS "Including library & headers with ${proname}")
        set(HAS_HEADERS On)
    else()
        set(HAS_HEADERS Off)
    endif()

    if (HAS_HEADERS AND WIN32)
        # this adds the .lib file on windows, which is needed for linking with the headers
        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname} ${CMAKE_CURRENT_SOURCE_DIR}/mod.json
            COMMAND ${SAPFIRE_CLI} package new ${CMAKE_CURRENT_SOURCE_DIR} 
                --binary $<TARGET_FILE:${proname}> $<TARGET_LINKER_FILE:${proname}>
                --output ${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}.sapfire
                ${INSTALL_ARG}
            VERBATIM USES_TERMINAL
        )
    else()
        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname} ${CMAKE_CURRENT_SOURCE_DIR}/mod.json
            COMMAND ${SAPFIRE_CLI} package new ${CMAKE_CURRENT_SOURCE_DIR} 
                --binary $<TARGET_FILE:${proname}>
                --output ${CMAKE_CURRENT_BINARY_DIR}/${MOD_ID}.sapfire
                ${INSTALL_ARG}
            VERBATIM USES_TERMINAL
        )
    endif()

    # Add dependency dir to include path
    if (EXISTS "${CMAKE_CURRENT_BINARY_DIR}/sapfire-deps")

        file(GLOB dirs ${CMAKE_CURRENT_BINARY_DIR}/sapfire-deps/*)

        set(libs_to_link "")

        # Iterate dependency directories
        foreach(dir ${dirs})
            if (IS_DIRECTORY ${dir})
                
                # v1.4.1 fixes optional dependencies
                if (${SAPFIRE_CLI_VERSION} VERSION_GREATER_EQUAL "1.4.1")
                    # Read dep info
                    file(READ "${dir}/sapfire-dep-options.json" DEP_JSON)
                    string(JSON required GET "${DEP_JSON}" "required")

                    # If this is not a required dependency, don't link it
                    if (NOT ${required})
                        continue()
                    endif()
                else()
                    message(WARNING
                        "You are using CLI v1.4.0, which has a bug with optional "
                        "dependencies - update to v1.4.1 if you want to use "
                        "optional dependencies!"
                    )
                endif()

                # Otherwise add all .libs or whatever the platform's library type is
                if (WIN32 OR LINUX)
                    file(GLOB libs ${dir}/*.lib)
                    list(APPEND libs_to_link ${libs})
                elseif (APPLE)
                    file(GLOB libs ${dir}/*.dylib)
                    list(APPEND libs_to_link ${libs})
                elseif (ANDROID)
                    if (CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a")
                        file(GLOB libs ${dir}/*.android64.so)
                    else()
                        file(GLOB libs ${dir}/*.android32.so)
                    endif()
                    list(APPEND libs_to_link ${libs})
                else()
                    message(FATAL_ERROR "Library extension not defined on this platform")
                endif()

            endif()
        endforeach()

        # Link libs
        target_include_directories(${proname} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/sapfire-deps")
        target_link_libraries(${proname} ${libs_to_link})
        
    endif()

    # Add package target + make output name the mod id
    set_target_properties(${proname} PROPERTIES PREFIX "")
    if (DEFINED SAPFIRE_MOD_BINARY_SUFFIX)
        set_target_properties(${proname} PROPERTIES SUFFIX ${SAPFIRE_MOD_BINARY_SUFFIX})
    endif()
    set_target_properties(${proname} PROPERTIES OUTPUT_NAME ${MOD_ID})
endfunction()

function(create_sapfire_file proname)
    # todo: deprecate at some point ig
    # message(DEPRECATION
    #     "create_sapfire_file has been replaced with setup_sapfire_mod - "
    #     "please replace the function call"
    # )

    # forward all args
    setup_sapfire_mod(${proname} ${ARGN})
endfunction()

function(package_sapfire_resources proname src dest)
    if (SAPFIRE_DISABLE_CLI_CALLS)
        message("Skipping packaging resources from ${src} into ${dest}")
        return()
    endif()

    message(STATUS "Packaging resources from ${src} into ${dest}")

    if(SAPFIRE_CLI STREQUAL "SAPFIRE_CLI-NOTFOUND")
        message(WARNING
            "package_sapfire_resources called, but Sapfire CLI was "
            "not found - You will need to manually package the resources"
        )
    else()

        add_custom_target(${proname}_PACKAGE ALL
            DEPENDS ${proname}
            COMMAND ${SAPFIRE_CLI} package resources ${src} ${dest}
            VERBATIM USES_TERMINAL
        )

    endif()
endfunction()

function(package_sapfire_resources_now proname src dest header_dest)
    if (SAPFIRE_DISABLE_CLI_CALLS)
        message(WARNING
            "package_sapfire_resources_now called, but SAPFIRE_DISABLE_CLI_CALLS 
            is set to true - Faking output result in case you only wish to 
            analyze the project statically, do not expect built project to 
            function properly"
        )
        set(HEADER_FILE
            "#include <unordered_map>\n\n"
            "static const std::unordered_map<std::string, std::string> "
            "LOADER_RESOURCE_HASHES {}\;\n"
        )
        file(WRITE ${header_dest} ${HEADER_FILE})
        message(STATUS "Wrote fake resource hashes to ${header_dest}")
        return()
    endif()

    if(SAPFIRE_CLI STREQUAL "SAPFIRE_CLI-NOTFOUND")
        message(FATAL_ERROR
            "package_sapfire_resources_now called, but Sapfire CLI "
            "was not found - Please install Sapfire CLI from "
            "https://github.com/sapfire-sdk/cli/releases/latest"
        )
        return()
    endif()
    
    message(STATUS "Packaging resources now from ${src} into ${dest}")

    execute_process(
        COMMAND ${SAPFIRE_CLI} package resources ${src} ${dest} --shut-up
        RESULT_VARIABLE SAPFIRE_PACKAGE_RES
        COMMAND_ERROR_IS_FATAL ANY
    )

    if (NOT SAPFIRE_PACKAGE_RES EQUAL "0")
        message(FATAL_ERROR
            "Command \"${SAPFIRE_CLI} package resources ${src} ${dest}\" returned "
            "${SAPFIRE_PACKAGE_RES} - Expected 0"
        )
    endif()

    file(GLOB RESOURCE_FILES "${dest}/*.*")

    set(HEADER_FILE
        "#include <unordered_map>\n\n"
        "static const std::unordered_map<std::string, std::string> "
        "LOADER_RESOURCE_HASHES {\n"
        # "#include <vector>\n\n"
        # "static const std::vector<std::string> "
        # "LOADER_RESOURCE_FILES {\n"
    )

    # yeah don't think we need to check too many stuff
    # list(APPEND HASHED_EXTENSIONS ".png")
    # list(APPEND HASHED_EXTENSIONS ".mp3")
    # list(APPEND HASHED_EXTENSIONS ".ogg")
    list(APPEND HASHED_TEXT_EXTENSIONS ".md")

    foreach(file ${RESOURCE_FILES})
        cmake_path(GET file FILENAME FILE_NAME)
        get_filename_component(FILE_EXTENSION ${file} EXT)

        list(FIND HASHED_EXTENSIONS "${FILE_EXTENSION}" FILE_SHOULD_HASH)

        if (NOT FILE_NAME STREQUAL ".sapfire_cache" AND NOT FILE_SHOULD_HASH EQUAL -1)
            
            file(SHA256 ${file} COMPUTED_HASH)
            file(SIZE ${file} FILE_SIZE)
            message(STATUS "Hashed ${file} to ${COMPUTED_HASH} (${FILE_SIZE} bytes)")
            list(APPEND HEADER_FILE "\t{ \"${FILE_NAME}\", \"${COMPUTED_HASH}\" },\n")

            # list(APPEND HEADER_FILE "\t\"${FILE_NAME}\",\n")

        endif()

        list(FIND HASHED_TEXT_EXTENSIONS "${FILE_EXTENSION}" FILE_SHOULD_TEXT_HASH)

        if (NOT FILE_NAME STREQUAL ".sapfire_cache" AND NOT FILE_SHOULD_TEXT_HASH EQUAL -1)
            
            # create list of lines form the contens of a file
            file(STRINGS ${file} LINES)
            list(JOIN LINES "" JOINED)
            # compute hash of the lines
            string(LENGTH "${JOINED}" FILE_SIZE)
            string(SHA256 COMPUTED_HASH "${JOINED}")
            
            message(STATUS "Hashed ${file} to ${COMPUTED_HASH} (${FILE_SIZE} bytes)")
            list(APPEND HEADER_FILE "\t{ \"${FILE_NAME}\", \"${COMPUTED_HASH}\" },\n")

            # list(APPEND HEADER_FILE "\t\"${FILE_NAME}\",\n")

        endif()

    endforeach()

    list(APPEND HEADER_FILE "}\;\n")

    file(WRITE ${header_dest} ${HEADER_FILE})
    message(STATUS "Wrote resource hashes to ${header_dest}")

endfunction()
