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
        ${CMAKE_SOURCE_DIR}/src/*/include # include all include directories of project components
        ${CMAKE_CURRENT_SOURCE_DIR}/src
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

# configure output directory
set_target_properties(run_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
