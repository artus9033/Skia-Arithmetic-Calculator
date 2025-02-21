find_package(Doxygen REQUIRED)

# set Doxygen variables
set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# configure Doxyfile
configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

# make docs depend on source files (to properly handle caching)
file(GLOB_RECURSE IMPL_SOURCE_FILES 
    "${CMAKE_SOURCE_DIR}/src/**/*"
)

set(SOURCE_FILES
    ${IMPL_SOURCE_FILES}
    ${CMAKE_SOURCE_DIR}/README.md
)

# copy the root README.md to the docs directory
configure_file(${CMAKE_SOURCE_DIR}/README.md ${CMAKE_CURRENT_SOURCE_DIR}/README.md COPYONLY)

# add docs target
add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating documentation with Doxygen"
    VERBATIM
    DEPENDS ${SOURCE_FILES}
)
