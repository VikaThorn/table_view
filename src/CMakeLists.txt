add_library(table_lib STATIC
    point_table.cpp
)
target_include_directories(table_lib PUBLIC ${PROJECT_SOURCE_DIR})