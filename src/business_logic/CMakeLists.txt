file(GLOB_RECURSE IMPL_SOURCE_FILES 
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(business_logic STATIC
    ${IMPL_SOURCE_FILES}
)

target_link_libraries(business_logic PUBLIC boost_multiprecision)

target_include_directories(business_logic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${spdlog_SOURCE_DIR}/include)

if(WIN32)
    # force static runtime linking
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -MTd")
endif()
