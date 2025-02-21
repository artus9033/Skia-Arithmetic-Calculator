cmake_minimum_required(VERSION 3.20)
project(FlowArithmeticCalculator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# third_party dependencies
add_subdirectory(third_party)

# project components
add_subdirectory(src/business_logic)
add_subdirectory(src/gui_app)

# add tests (if enabled)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# configure Doxygen (if enabled)
if(BUILD_DOCS)
    find_package(Doxygen REQUIRED)

    # set Doxygen variables
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # configure Doxyfile
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # add docs target
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation with Doxygen"
        VERBATIM
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/docs/mainpage.md
    )

    # make docs depend on source files (to properly handle caching)
    file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.hpp
        ${CMAKE_SOURCE_DIR}/include/*.hpp
    )
    add_custom_command(
        TARGET docs
        DEPENDS ${SOURCE_FILES}
    )
endif()
