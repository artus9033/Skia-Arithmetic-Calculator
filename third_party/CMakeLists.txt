include(FetchContent)

# set static build for all dependencies
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

# Boost (multiprecision only)
FetchContent_Declare(
    boost
    URL https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz
    URL_HASH SHA256=f55c340aa49763b1925ccf02b2e83f35fdcf634c9d5164a2acb87540173c741d
)
set(BOOST_INCLUDE_LIBRARIES multiprecision)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_MakeAvailable(boost)

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
function(fetch_skia)
    if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/skia)
        # Clone Skia
        execute_process(
            COMMAND git clone https://skia.googlesource.com/skia.git --branch=canvaskit/0.38.2 --single-branch
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
        
        # Get dependencies
        execute_process(
            COMMAND python tools/git-sync-deps
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/skia
        )

        # Configure GN build with static linking
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

        execute_process(
            COMMAND python tools/git-sync-deps
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/skia
        )

        execute_process(
            COMMAND bin/gn gen out/Release --args=${SKIA_BUILD_ARGS_STR}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/skia
        )

        # build Skia
        execute_process(
            COMMAND ninja -C out/Release
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/skia
        )
    endif()

    # create imported target for Skia
    add_library(skia STATIC IMPORTED GLOBAL)
    
    if(WIN32)
        set_target_properties(skia PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/skia/out/Release/skia.lib
        )
    else()
        set_target_properties(skia PROPERTIES
            IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/skia/out/Release/libskia.a
        )
    endif()

    set_target_properties(skia PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR}/skia
    )
endfunction()

fetch_skia()
