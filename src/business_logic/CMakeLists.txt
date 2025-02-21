add_library(business_logic STATIC
    src/calculator.cpp
    src/flow_graph.cpp
)

target_include_directories(business_logic
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/business_logic
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(business_logic
    PUBLIC
        Boost::boost  # Only header-only multiprecision library needed
)

target_compile_features(business_logic PUBLIC cxx_std_23)
