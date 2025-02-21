include(FetchContent)

# set static build for all dependencies
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

# Boost
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 EXACT REQUIRED)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
)
set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Skia + configuration for static linkage GN build
if(NOT EXISTS ${FETCHCONTENT_BASE_DIR}/skia)
    message("Cloning Skia to ${FETCHCONTENT_BASE_DIR}/skia, this may take a while...")

    # clone Skia
    execute_process(
        COMMAND git clone https://skia.googlesource.com/skia.git --branch=canvaskit/0.38.2 --single-branch
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}
    )
    
    # get dependencies
    execute_process(
        COMMAND python tools/git-sync-deps
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}/skia
    )
else()
    message("Skia already cloned to ${FETCHCONTENT_BASE_DIR}/skia")
endif()

set(SKIA_SHARED_LIB ${FETCHCONTENT_BASE_DIR}/skia/out/Release/libskia.a)

if(NOT EXISTS ${SKIA_SHARED_LIB})
    message("${SKIA_SHARED_LIB} does not exist -> will prepare & build Skia")

    # configure GN build with static linking
    set(SKIA_BUILD_ARGS "is_official_build=true"
        "is_debug=false"
        "is_component_build=false"
        "skia_use_system_libraries=false"
        "skia_use_gl=true"
        "skia_enable_gpu=true"
        "skia_enable_tools=false"
        "skia_use_system_expat=false"
        "skia_use_system_libjpeg_turbo=false"
        "skia_use_system_libpng=false"
        "skia_use_system_libwebp=false"
        "skia_use_system_zlib=false"
    )

    if(WIN32)
        list(APPEND SKIA_BUILD_ARGS "target_os=\"windows\"" "cc=\"clang\"" "cxx=\"clang++\"")
    elseif(APPLE)
        list(APPEND SKIA_BUILD_ARGS "target_os=\"mac\"")
    else()
        list(APPEND SKIA_BUILD_ARGS "target_os=\"linux\"")
    endif()

    string(JOIN " " SKIA_BUILD_ARGS_STR ${SKIA_BUILD_ARGS})

    message("Syncing Skia dependencies, this may take a while...")
    execute_process(
        COMMAND python tools/git-sync-deps
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}/skia
    )

    message("Configuring Skia build with gn, this may take a while...")
    execute_process(
        COMMAND bin/gn gen out/Release --args=${SKIA_BUILD_ARGS_STR}
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}/skia
    )

    # build Skia
    message("Building Skia with ninja, this may take a while...")
    execute_process(
        COMMAND ninja -C out/Release
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}/skia
    )
else()
    message("Skia already built to ${SKIA_SHARED_LIB}")
endif()

# create imported target for Skia
add_library(skia STATIC IMPORTED GLOBAL)

if(WIN32)
    set_target_properties(skia PROPERTIES
        IMPORTED_LOCATION ${FETCHCONTENT_BASE_DIR}/skia/out/Release/skia.lib
    )
else()
    set_target_properties(skia PROPERTIES
        IMPORTED_LOCATION ${FETCHCONTENT_BASE_DIR}/skia/out/Release/libskia.a
    )
endif()

set_target_properties(skia PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${FETCHCONTENT_BASE_DIR}/skia
)
