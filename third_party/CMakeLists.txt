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
        COMMAND git clone https://skia.googlesource.com/skia.git
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}
    )

    execute_process(
        COMMAND git checkout cca9328df6ca5f636801cc625992a1f7c34e0d66
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}/skia
    )
    
    # get dependencies
    execute_process(
        COMMAND python tools/git-sync-deps
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}/skia
    )
else()
    message("\nSkia already cloned to ${FETCHCONTENT_BASE_DIR}/skia")
endif()

set(SKIA_SHARED_LIB ${FETCHCONTENT_BASE_DIR}/skia/out/Release/libskia.a)

if(NOT EXISTS ${SKIA_SHARED_LIB})
    message("\n(!) ${SKIA_SHARED_LIB} does not exist -> will prepare & build Skia")

    # configure GN build with static linking
    set(SKIA_BUILD_ARGS "is_official_build=true"
        "is_debug=false"
        "is_component_build=false"
        "skia_use_gl=true"
        "skia_enable_gpu=true"
        "skia_enable_tools=false"
        "skia_use_system_expat=false"
        "skia_use_system_libjpeg_turbo=false"
        "skia_use_system_libpng=false"
        "skia_use_system_libwebp=false"
        "skia_use_system_zlib=false"
        "skia_use_icu=true"
        "skia_use_system_icu=false"
    )

    if(WIN32)
        list(APPEND SKIA_BUILD_ARGS "target_os=\"windows\"" "cc=\"clang\"" "cxx=\"clang++\"")
    elseif(APPLE)
        list(APPEND SKIA_BUILD_ARGS "target_os=\"mac\"")
    else()
        list(APPEND SKIA_BUILD_ARGS "target_os=\"linux\"")
    endif()

    if (APPLE)
        list(APPEND SKIA_BUILD_ARGS "skia_use_sfntly=false skia_use_freetype=true skia_use_harfbuzz=true skia_pdf_subset_harfbuzz=true skia_use_system_freetype2=false skia_use_system_harfbuzz=false")
    endif()

    string(JOIN " " SKIA_BUILD_ARGS_STR ${SKIA_BUILD_ARGS})

    message("\nSyncing Skia dependencies, this may take a while...")
    execute_process(
        COMMAND python tools/git-sync-deps
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}/skia
    )

    message("\nConfiguring Skia build with gn, this may take a while...")
    execute_process(
        COMMAND bin/gn gen out/Release --args=${SKIA_BUILD_ARGS_STR}
        WORKING_DIRECTORY ${FETCHCONTENT_BASE_DIR}/skia
    )

    # build Skia
    message("\nBuilding Skia with ninja, this may take a while...")
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

target_include_directories(skia INTERFACE ${FETCHCONTENT_BASE_DIR}) # allow includes in style of skia/include/core/SkCanvas.h
target_include_directories(skia INTERFACE ${FETCHCONTENT_BASE_DIR}/skia) # allow includes in style of include/core/SkCanvas.h
