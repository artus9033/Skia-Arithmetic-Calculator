# enable testing for this directory and its children
enable_testing()

# collect all test source files
file(GLOB_RECURSE TEST_SOURCES 
    "src/*.cpp"
)

# create test executable
add_executable(run_tests ${TEST_SOURCES})

# set include directories
target_include_directories(run_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    SYSTEM INTERFACE
        ${Boost_INCLUDE_DIRS}
)

# discover tests
include(GoogleTest)
gtest_discover_tests(run_tests)

# link against business logic and GTest
target_link_libraries(run_tests
    PRIVATE
        business_logic
        GTest::gtest
        GTest::gtest_main
        ${Boost_LIBRARIES}
)
# treat the linked libraries' headers as system interface for clang-tidy not to check them
target_include_directories(
    gui_app SYSTEM INTERFACE
        $<TARGET_PROPERTY:GTest::gtest,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:GTest::gtest_main,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:${Boost_LIBRARIES},INTERFACE_INCLUDE_DIRECTORIES>
)

# configure output directory
set_target_properties(run_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
