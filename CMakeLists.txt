cmake_minimum_required(VERSION 3.20)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# for clang-tidy and cppcheck
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_PARALLEL_LEVEL 12 CACHE STRING "Number of parallel build jobs" FORCE)
if(UNIX)
    # linux and Mac - use clang
    set(CMAKE_C_COMPILER "clang")
    set(CMAKE_CXX_COMPILER "clang++")

    # enable all warnings and treat them as errors
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

    # Debug flags for non-MSVC compilers
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    
    # Release flags for non-MSVC compilers (max optimization)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
else()
    # windows - use clang-cl
    set(CMAKE_C_COMPILER "clang-cl")
    set(CMAKE_CXX_COMPILER "clang-cl")

    # force static runtime linking
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MTd")

    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # if windows, disable -Wmicrosoft-include and enable exceptions explicitly
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmicrosoft-include -EHsc")
endif()

add_definitions(-DSPDLOG_NO_EXCEPTIONS)

project(FlowArithmeticCalculator VERSION 1.0.0 LANGUAGES CXX)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    # resolve warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake >= 3.24:
    # set policy to mark extracted files at extraction time instead of using timestamps from the archive
    cmake_policy(SET CMP0135 NEW)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.31")
    # resolve warning about CMP0175 in CMake >= 3.31:
    # explicitly configure behaviour of add_custom_command() with invalid arguments
    cmake_policy(SET CMP0175 NEW)
endif()

# third_party dependencies - include as-is in this file
include(third_party/CMakeLists.txt)

# define SPDLOG_ACTIVE_LEVEL based on the active variant for main.cpp to set it
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DSET_LOG_ACTIVE_LEVEL=spdlog::level::debug)
else()
    add_definitions(-DSET_LOG_ACTIVE_LEVEL=spdlog::level::info)
endif()

# custom build options that can be passed to the project
option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" ON)

# FetchContent for dependency management
include(FetchContent)
# CTest for testing
include(CTest)

add_subdirectory(src/business_logic)
add_subdirectory(src/gui_app)

# add tests (if enabled)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# configure Doxygen (if enabled)
if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

# install pre-commit & pre-push hooks if git is available and only on UNIXes
if(NOT GIT_FOUND)
  find_package(Git QUIET)
endif()

if(UNIX AND GIT_FOUND AND EXISTS ${PROJECT_SOURCE_DIR}/.git)
    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/.git/hooks/pre-commit)
        message(STATUS "Installing pre-commit hook")

        configure_file(
            ${CMAKE_SOURCE_DIR}/pre-commit.in
            ${PROJECT_SOURCE_DIR}/.git/hooks/pre-commit
            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )
    endif()

    if(NOT EXISTS ${PROJECT_SOURCE_DIR}/.git/hooks/pre-push)
        message(STATUS "Installing pre-push hook")

        configure_file(
            ${CMAKE_SOURCE_DIR}/pre-push.in
            ${PROJECT_SOURCE_DIR}/.git/hooks/pre-push
            FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        )
    endif()
endif()