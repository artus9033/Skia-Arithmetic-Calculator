file(GLOB_RECURSE IMPL_SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
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

add_executable(gui_app
    ${IMPL_SOURCE_FILES}
)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
target_link_libraries(gui_app PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
# treat the linked libraries' headers as system interface for clang-tidy not to check them
target_include_directories(
    gui_app SYSTEM INTERFACE
        $<TARGET_PROPERTY:Qt6::Core,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Qt6::Gui,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Qt6::Widgets,INTERFACE_INCLUDE_DIRECTORIES>
)

target_include_directories(gui_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(gui_app
    PRIVATE
        business_logic
        glfw
        skia
)
# treat the linked libraries' headers as system interface for clang-tidy not to check them
target_include_directories(
    gui_app SYSTEM INTERFACE
        $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:skia,INTERFACE_INCLUDE_DIRECTORIES>
)

# if linux
if(UNIX AND NOT APPLE)
    find_package(Freetype REQUIRED)
    find_package(XCB REQUIRED)
    target_link_libraries(gui_app PUBLIC Freetype::Freetype XCB::XCB)
    # treat the linked libraries' headers as system interface for clang-tidy not to check them
    target_include_directories(
        gui_app SYSTEM INTERFACE
            $<TARGET_PROPERTY:Freetype::Freetype,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:XCB::XCB,INTERFACE_INCLUDE_DIRECTORIES>
    )
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
    # treat the linked libraries' headers as system interface for clang-tidy not to check them
    target_include_directories(
        gui_app SYSTEM INTERFACE
            $<TARGET_PROPERTY:Cocoa,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:OpenGL,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:IOKit,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:OpenGL,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:CoreVideo,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:QuartzCore,INTERFACE_INCLUDE_DIRECTORIES>
    )
endif()
