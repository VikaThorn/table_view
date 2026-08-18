file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS "*.cpp")

if(EXAMPLE_SOURCES)
    add_executable(example ${EXAMPLE_SOURCES})
    target_link_libraries(example PRIVATE table_lib)
endif()