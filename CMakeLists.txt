cmake_minimum_required(VERSION 3.20)
project(FlowArithmeticCalculator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    # resolve warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake >= 3.24:
    # set policy to mark extracted files at extraction time instead of using timestamps from the archive
    cmake_policy(SET CMP0135 NEW)

    # resolve warning about CMP0175 in CMake >= 3.24:
    # explicitly configure behaviour of add_custom_command() with invalid arguments
    cmake_policy(SET CMP0175 NEW)
endif()

# by default, build in release mode
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# custom build options that can be passed to the project
option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" ON)

# enforce static linking
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

# FetchContent for dependency management
include(FetchContent)
# CTest for testing
include(CTest)

# third_party dependencies - include as-is in this file
include(third_party/CMakeLists.txt)

# propagate third party dependencies to project components
link_libraries(${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIRS})

# project components
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
