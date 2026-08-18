#include <catch2/catch_test_macros.hpp>

#include <array>
#include <string>
#include <vector>

#include <array_table.h>

using table_fabric::ArrayTable;

TEST_CASE("ArrayTable: basic counts from vector<array<double, 3>>", "[array_table]") {
    std::vector<std::array<double, 3>> data = {
        {1.5, 2.0, 3.7},
        {4.0, 5.1, 6.0},
    };
    ArrayTable<double, 3> table(data);

    REQUIRE(table.row_count() == 2);
    REQUIRE(table.column_count() == 3);
}

TEST_CASE("ArrayTable: add_row with double", "[array_table]") {
    std::vector<std::array<double, 3>> data;
    ArrayTable<double, 3> table(data);

    REQUIRE(table.add_row({"1.5", "2.0", "3.7"}));
    REQUIRE(table.row_count() == 1);

    REQUIRE(!table.add_row({"1.5", "abc", "3.7"}));
    REQUIRE(table.row_count() == 1);

    REQUIRE(!table.add_row({"1.5", "2.0"}));
    REQUIRE(table.row_count() == 1);
}

TEST_CASE("ArrayTable: add_row with string", "[array_table]") {
    std::vector<std::array<std::string, 4>> data;
    ArrayTable<std::string, 4> table(data);

    REQUIRE(table.add_row({"Alice", "30", "Moscow", "Engineer"}));
    REQUIRE(table.row_count() == 1);
}

TEST_CASE("ArrayTable: set_value for double", "[array_table]") {
    std::vector<std::array<double, 3>> data = {{1.0, 2.0, 3.0}};
    ArrayTable<double, 3> table(data);

    REQUIRE(table.set_value(0, 1, "42.5"));
    REQUIRE(!table.set_value(0, 1, "abc"));
    REQUIRE(!table.set_value(10, 0, "1.0"));
}

TEST_CASE("ArrayTable: set_value for string", "[array_table]") {
    std::vector<std::array<std::string, 2>> data = {{"a", "b"}};
    ArrayTable<std::string, 2> table(data);

    REQUIRE(table.set_value(0, 0, "new_value"));
}

TEST_CASE("ArrayTable: remove_row", "[array_table]") {
    std::vector<std::array<int, 2>> data = {{1, 2}, {3, 4}};
    ArrayTable<int, 2> table(data);

    REQUIRE(table.remove_row(0));
    REQUIRE(table.row_count() == 1);
    REQUIRE(!table.remove_row(10));
}

TEST_CASE("ArrayTable: save_to_csv with double", "[array_table]") {
    std::vector<std::array<double, 3>> data = {{1.5, 2.0, 3.7}};
    ArrayTable<double, 3> table(data);

    REQUIRE(table.save_to_csv("test_array_double_output.csv"));
}

TEST_CASE("ArrayTable: save_to_csv with string", "[array_table]") {
    std::vector<std::array<std::string, 2>> data = {{"Alice", "Bob"}};
    ArrayTable<std::string, 2> table(data);

    REQUIRE(table.save_to_csv("test_array_string_output.csv"));
}