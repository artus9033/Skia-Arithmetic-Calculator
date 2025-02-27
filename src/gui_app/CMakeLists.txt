file(GLOB_RECURSE IMPL_SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


if (APPLE)
    execute_process(
        COMMAND brew --prefix qt@6
        OUTPUT_VARIABLE QT_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${QT_PREFIX}")
endif()

# Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
link_libraries(Qt6::Core Qt6::Gui Qt6::Widgets)


add_executable(gui_app
    ${IMPL_SOURCE_FILES}
)

target_include_directories(gui_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${MAGIC_ENUM_INCLUDE_DIR})

target_link_libraries(gui_app
    PRIVATE
        business_logic
        glfw
        skia
)


# if linux
if(UNIX AND NOT APPLE)
    find_package(Freetype REQUIRED)
    find_package(XCB REQUIRED)
    target_link_libraries(gui_app PUBLIC Freetype::Freetype XCB::XCB)
endif()

# ensure static linkage on Windows
if(WIN32)
    target_link_options(gui_app PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )
endif()

# set output directory
set_target_properties(gui_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# link required Skia peer dependencies for macOS
if(APPLE)
    target_link_libraries(gui_app PRIVATE
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework QuartzCore"
    )
endif()
