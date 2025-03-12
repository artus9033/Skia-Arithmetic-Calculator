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

# include the Qt6 CMake config file, if it exists and we are on Windows
if (WIN32 AND EXISTS config-windows.cmake)
    message(STATUS "Including Windows-specific configuration file: config-windows.cmake")

    include(${PROJECT_SOURCE_DIR}/config-windows.cmake)

    message(STATUS "Qt6_DIR: ${Qt6_DIR}")
endif()

# Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
target_link_libraries(gui_app PUBLIC Qt6::Core Qt6::Gui Qt6::Widgets)
# treat the linked libraries' headers as system interface for clang-tidy not to check them
target_include_directories(
    gui_app SYSTEM INTERFACE
        $<TARGET_PROPERTY:Qt6::Core,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Qt6::Gui,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Qt6::Widgets,INTERFACE_INCLUDE_DIRECTORIES>
)

target_include_directories(gui_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/src SYSTEM INTERFACE ${spdlog_SOURCE_DIR}/include)

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
target_link_libraries(gui_app PUBLIC Freetype::Freetype XCB::XCB fontconfig)
# treat the linked libraries' headers as system interface for clang-tidy not to check them
target_include_directories(
    gui_app SYSTEM INTERFACE
        $<TARGET_PROPERTY:Freetype::Freetype,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:XCB::XCB,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:fontconfig,INTERFACE_INCLUDE_DIRECTORIES>
)
endif()

# set output directory
set_target_properties(gui_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# link required Skia peer dependencies for macOS
if (APPLE)
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
            $<TARGET_PROPERTY:CoreVideo,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:QuartzCore,INTERFACE_INCLUDE_DIRECTORIES>
    )
endif()

if (WIN32)
    target_link_libraries(gui_app PRIVATE
        OpenGL32.lib
    )

    if(Qt6_FOUND AND WIN32 AND TARGET Qt6::qmake AND NOT TARGET Qt6::windeployqt)
        get_target_property(_qt6_qmake_location Qt6::qmake IMPORTED_LOCATION)
        execute_process(
            COMMAND "${_qt6_qmake_location}" -query QT_INSTALL_PREFIX
            RESULT_VARIABLE return_code
            OUTPUT_VARIABLE qt6_install_prefix
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        set(imported_location "${qt6_install_prefix}/bin/windeployqt.exe")
        if(NOT EXISTS ${imported_location})
            get_filename_component(_qt6_qmake_bin_dir "${_qt6_qmake_location}" DIRECTORY)
            set(imported_location "${_qt6_qmake_bin_dir}/windeployqt.exe")
    endif()
        if(EXISTS ${imported_location})
            add_executable(Qt6::windeployqt IMPORTED)
            set_target_properties(Qt6::windeployqt PROPERTIES
                IMPORTED_LOCATION ${imported_location}
            )
        endif()
    endif()

    if(TARGET Qt6::windeployqt)
        add_custom_command(TARGET gui_app
            POST_BUILD
            COMMAND set PATH=%PATH%$<SEMICOLON>${qt6_install_prefix}/bin
            COMMAND Qt6::windeployqt --dir "${CMAKE_BINARY_DIR}/bin" "${CMAKE_BINARY_DIR}/bin/gui_app.exe" || dir >nul:
        )
    endif()
endif()
