#include <catch2/catch_test_macros.hpp>

#include <array>
#include <list>
#include <string>
#include <vector>

#include <array_table.h>

using table_fabric::ArrayTable;

TEST_CASE("ArrayTable: basic counts from vector<array<double, 3>>", "[array_table]") {
    std::vector<std::array<double, 3>> data = {
        {1.5, 2.0, 3.7},
        {4.0, 5.1, 6.0},
    };
    ArrayTable<std::vector<std::array<double, 3>>> table(data);

    REQUIRE(table.row_count() == 2);
    REQUIRE(table.column_count() == 3);
}

TEST_CASE("ArrayTable: view reflects changes in original vector", "[array_table]") {
    std::vector<std::array<double, 3>> data = {{1.0, 2.0, 3.0}};
    ArrayTable<std::vector<std::array<double, 3>>> table(data);

    REQUIRE(table.set_value(0, 1, "42.5"));
    REQUIRE(data[0][1] == 42.5);
}

TEST_CASE("ArrayTable: add_row reflects in original vector", "[array_table]") {
    std::vector<std::array<double, 3>> data;
    ArrayTable<std::vector<std::array<double, 3>>> table(data);

    REQUIRE(table.add_row({"1.5", "2.0", "3.7"}));
    REQUIRE(data.size() == 1);
    REQUIRE(data[0][0] == 1.5);
    REQUIRE(data[0][2] == 3.7);
}

TEST_CASE("ArrayTable: add_row with string reflects in original", "[array_table]") {
    std::vector<std::array<std::string, 4>> data;
    ArrayTable<std::vector<std::array<std::string, 4>>> table(data);

    REQUIRE(table.add_row({"Alice", "30", "Moscow", "Engineer"}));
    REQUIRE(data.size() == 1);
    REQUIRE(data[0][0] == "Alice");
}

TEST_CASE("ArrayTable: add_row with invalid number leaves original unchanged", "[array_table]") {
    std::vector<std::array<double, 3>> data;
    ArrayTable<std::vector<std::array<double, 3>>> table(data);

    REQUIRE(!table.add_row({"1.5", "abc", "3.7"}));
    REQUIRE(data.empty());
}

TEST_CASE("ArrayTable: set_value with invalid number leaves original unchanged", "[array_table]") {
    std::vector<std::array<double, 3>> data = {{1.0, 2.0, 3.0}};
    ArrayTable<std::vector<std::array<double, 3>>> table(data);

    REQUIRE(!table.set_value(0, 1, "abc"));
    REQUIRE(data[0][1] == 2.0);
}

TEST_CASE("ArrayTable: remove_row reflects in original", "[array_table]") {
    std::vector<std::array<int, 2>> data = {{1, 2}, {3, 4}};
    ArrayTable<std::vector<std::array<int, 2>>> table(data);

    REQUIRE(table.remove_row(0));
    REQUIRE(data.size() == 1);
    REQUIRE(data[0][0] == 3);
}

TEST_CASE("ArrayTable: view works with list", "[array_table]") {
    std::list<std::array<int, 2>> data = {{1, 2}, {3, 4}};
    ArrayTable<std::list<std::array<int, 2>>> table(data);

    REQUIRE(table.row_count() == 2);
    REQUIRE(table.set_value(0, 0, "99"));
    REQUIRE(data.front()[0] == 99);
}

TEST_CASE("ArrayTable: save_to_csv with double", "[array_table]") {
    std::vector<std::array<double, 3>> data = {{1.5, 2.0, 3.7}};
    ArrayTable<std::vector<std::array<double, 3>>> table(data);

    REQUIRE(table.save_to_csv("test_array_double_output.csv"));
}

TEST_CASE("ArrayTable: save_to_csv with string", "[array_table]") {
    std::vector<std::array<std::string, 2>> data = {{"Alice", "Bob"}};
    ArrayTable<std::vector<std::array<std::string, 2>>> table(data);

    REQUIRE(table.save_to_csv("test_array_string_output.csv"));
}