include(FetchContent)
FetchContent_Declare(
    Catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.5.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

if(TEST_SOURCES)
    add_executable(tests ${TEST_SOURCES})
    target_link_libraries(tests PRIVATE table_lib Catch2::Catch2WithMain)

    include(Catch)
    catch_discover_tests(tests)
endif()