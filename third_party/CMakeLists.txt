include(FetchContent)

# set static build for all dependencies
set(BUILD_SHARED_LIBS OFF BOOL "Build shared libraries")

# Windows specific configuration
if(WIN32)
    # make it discover packages installed with winget
    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/Program Files/Doxygen;C:/Program Files (x86)/Boost;C:/Program Files (x86)/Doxygen")
endif()

# ensure python3 is on the PATH - otherwise Skia will fail to configure
find_program(PYTHON_EXECUTABLE python3)

if(NOT PYTHON_EXECUTABLE)
    message(FATAL_ERROR "python3 is not in the PATH! Without it, Skia will fail to configure. Please make sure it's installed and available, or if you have python, make sure to symlink an alias for it named python3.")
else()
    message(STATUS "python3 found at: ${PYTHON_EXECUTABLE}")
endif()

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# allow FetchContent_Populate on purpose not to include the project, just clone it
if(POLICY CMP0169)
    cmake_policy(SET CMP0169 OLD)
endif()

# show progress when GIT_PROGRESS is TRUE
Set(FETCHCONTENT_QUIET FALSE)

# Boost
message("Fetching Boost, this may take a while...")
FetchContent_Declare(
    Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.83.0
    GIT_PROGRESS TRUE
)
# FetchContent_MakeAvailable(Boost)
FetchContent_GetProperties(Boost)
if(NOT Boost_POPULATED)
  # fetch the content using previously declared details
  FetchContent_Populate(Boost)
endif()

set(Boost_INCLUDE_DIRS)

# for all subdirectories of FETCHCONTENT_BASE_DIR/boost-src/libs/ append all folders' paths + /include/ to Boost_INCLUDE_DIRS
file(GLOB BOOST_PACKAGES ${FETCHCONTENT_BASE_DIR}/boost-src/libs/*)

# iterate lib dirs and add their include dir + possibly their subpackages if present
foreach(DIR ${BOOST_PACKAGES})
   set(INCLUDE_DIR "${DIR}/include")

   list(APPEND Boost_INCLUDE_DIRS ${INCLUDE_DIR})

   file(GLOB SUBPACKAGES ${DIR}/*/include)

   list(APPEND Boost_INCLUDE_DIRS ${SUBPACKAGES})
endforeach()

# GLFW
message("Fetching GLFW, this may take a while...")
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
    GIT_PROGRESS TRUE
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# spd
message("Fetching spdlog, this may take a while...")
FetchContent_Declare(
    spd
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.1
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(spd)

# Google Test
message("Fetching GTest, this may take a while...")
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
    GIT_PROGRESS TRUE
)
set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Skia + configuration for static linkage GN build
set(SKIA_CLONE_PATH ${FETCHCONTENT_BASE_DIR}/skia)
if(NOT EXISTS ${SKIA_CLONE_PATH})
    message("Cloning Skia to ${SKIA_CLONE_PATH}, this may take a while...")

    # clone Skia
    execute_process(
        COMMAND git clone https://skia.googlesource.com/skia.git
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}
    )

    execute_process(
        COMMAND git checkout cca9328df6ca5f636801cc625992a1f7c34e0d66
        WORKING_DIRECTORY ${SKIA_CLONE_PATH}
    )
    
    # get dependencies
    execute_process(
        COMMAND python tools/git-sync-deps
        WORKING_DIRECTORY ${SKIA_CLONE_PATH}
    )
else()
    message("\nSkia already cloned to ${SKIA_CLONE_PATH}")
endif()

# if debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SKIA_OUT_DIR "Debug")
else()
    set(SKIA_OUT_DIR "Release")
endif()

if(UNIX)
    set(SKIA_SHARED_LIB ${SKIA_CLONE_PATH}/out/${SKIA_OUT_DIR}/libskia.a)
else()
    set(SKIA_SHARED_LIB ${SKIA_CLONE_PATH}/out/${SKIA_OUT_DIR}/skia.lib)
endif()

if(NOT EXISTS ${SKIA_SHARED_LIB})
    message("\n(!) ${SKIA_SHARED_LIB} does not exist -> will prepare & build Skia")

    # configure GN build
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SKIA_BUILD_ARGS "is_official_build=false"
            "is_debug=true"
            "extra_cflags=[\"-MTd\"]"
        )
    else()
        set(SKIA_BUILD_ARGS "is_official_build=true"
            "is_debug=false"
        )
    endif()

    list(APPEND SKIA_BUILD_ARGS "is_component_build=false skia_use_gl=true skia_enable_gpu=true")

    if(APPLE)
        list(APPEND SKIA_BUILD_ARGS "target_os=\"mac\"")
    elseif(UNIX)
        list(APPEND SKIA_BUILD_ARGS "target_os=\"linux\"")
    else()
        list(APPEND SKIA_BUILD_ARGS "target_os=\"win\"")
    endif()

    if(UNIX)
        # use clang
        list(APPEND SKIA_BUILD_ARGS "cc=\"clang\" cxx=\"clang++\"")

        # compilation flags - mix of system vs bundled libraries
        list(APPEND SKIA_BUILD_ARGS "skia_enable_tools=false skia_use_system_expat=false skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_icu=true skia_use_system_icu=false skia_use_harfbuzz=true skia_pdf_subset_harfbuzz=true skia_use_system_harfbuzz=false")

        if(APPLE)
            list(APPEND SKIA_BUILD_ARGS "skia_use_freetype=false")
        else()
            list(APPEND SKIA_BUILD_ARGS "skia_use_freetype=true skia_enable_fontmgr_fontconfig=true skia_use_fontconfig=true")
        endif()
    else()
        # windows

        # use clang-cl
        list(APPEND SKIA_BUILD_ARGS "clang_win=\"C:\\Program Files\\LLVM\" cc=\"clang-cl\" cxx=\"clang-cl\"")

        # compilation flags - use bundled libraries (not system ones)
        list(APPEND SKIA_BUILD_ARGS "skia_use_system_libjpeg_turbo=false skia_use_system_libpng=false skia_use_system_libwebp=false skia_use_system_zlib=false skia_use_libjpeg_turbo_decode=true skia_use_libjpeg_turbo_encode=true skia_use_system_zlib=false skia_use_zlib=true skia_use_icu=true skia_use_client_icu=false skia_use_libgrapheme=false skia_use_icu4x=false skia_use_system_icu=false skia_use_harfbuzz=true skia_use_system_harfbuzz=false skia_use_expat=false")
    endif()

    string(JOIN " " SKIA_BUILD_ARGS_STR ${SKIA_BUILD_ARGS})

    message("\nSyncing Skia dependencies, this may take a while...")
    execute_process(
        COMMAND python tools/git-sync-deps
        WORKING_DIRECTORY ${SKIA_CLONE_PATH}
        ERROR_VARIABLE cmdError
        OUTPUT_VARIABLE cmdOutput
        RESULT_VARIABLE cmdResult
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "${cmdOutput}")
    if(result)
        message(WARNING "Command failed - exit code: ${cmdResult}")
        message(WARNING "Error: ${cmdError}")
    endif()

    message("\nConfiguring Skia build with gn, this may take a while...")
    message("GN flags: ${SKIA_BUILD_ARGS_STR}")
    execute_process(
        COMMAND bin/gn gen out/${SKIA_OUT_DIR} --args=${SKIA_BUILD_ARGS_STR}
        WORKING_DIRECTORY ${SKIA_CLONE_PATH}
        ERROR_VARIABLE cmdError
        OUTPUT_VARIABLE cmdOutput
        RESULT_VARIABLE cmdResult
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "${cmdOutput}")
    if(result)
        message(WARNING "Command failed - exit code: ${cmdResult}")
        message(WARNING "Error: ${cmdError}")
    endif()

    # build Skia
    message("\nBuilding Skia with ninja, this may take a while...")
    execute_process(
        COMMAND ninja -C out/${SKIA_OUT_DIR}
        WORKING_DIRECTORY ${SKIA_CLONE_PATH}
        ERROR_VARIABLE cmdError
        OUTPUT_VARIABLE cmdOutput
        RESULT_VARIABLE cmdResult
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_STRIP_TRAILING_WHITESPACE
    )

    message(STATUS "${cmdOutput}")
    if(result)
        message(WARNING "Command failed - exit code: ${cmdResult}")
        message(WARNING "Error: ${cmdError}")
    endif()
else()
    message("Skia already built to ${SKIA_SHARED_LIB}")
endif()

# create imported target for Skia
add_library(skia STATIC IMPORTED GLOBAL)

if(WIN32)
    set_target_properties(skia PROPERTIES
        IMPORTED_LOCATION ${SKIA_CLONE_PATH}/out/${SKIA_OUT_DIR}/skia.lib
    )
else()
    set_target_properties(skia PROPERTIES
        IMPORTED_LOCATION ${SKIA_SHARED_LIB}
    )
endif()

get_filename_component(SKIA_CLONE_PARENT_DIR "${SKIA_CLONE_PATH}/.." ABSOLUTE)
target_include_directories(skia SYSTEM INTERFACE ${SKIA_CLONE_PARENT_DIR}) # allow includes in style of skia/include/core/SkCanvas.h
target_include_directories(skia SYSTEM INTERFACE ${SKIA_CLONE_PATH}) # allow includes in style of include/core/SkCanvas.h

if(APPLE)
    execute_process(
        COMMAND brew --prefix qt@6
        OUTPUT_VARIABLE QT_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT_PREFIX}")
endif()

# magic_enum
message("Fetching magic_enum, this may take a while...")
FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.7
)
FetchContent_MakeAvailable(magic_enum)

# set var with include dir for magic_enum
# set(MAGIC_ENUM_INCLUDE_DIR ${FETCHCONTENT_BASE_DIR}/magic_enum-src/include)
include_directories(SYSTEM INTERFACE ${FETCHCONTENT_BASE_DIR}/magic_enum-src/include)
