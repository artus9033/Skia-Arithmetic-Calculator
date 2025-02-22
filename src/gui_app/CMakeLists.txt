file(GLOB_RECURSE IMPL_SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(gui_app
    ${IMPL_SOURCE_FILES}
)

target_include_directories(gui_app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(gui_app
    PRIVATE
        business_logic
        glfw
        skia
)

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
