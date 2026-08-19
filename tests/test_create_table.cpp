#include <catch2/catch_test_macros.hpp>

#include <array>
#include <list>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include <point.h>
#include <table_fabric.h>

using table_fabric::Point;
using table_fabric::create_table;

TEST_CASE("create_table: provider type 1 (Point containers)", "[create_table]") {
    std::vector<Point> vec;
    std::list<Point> lst;
    std::set<Point> st;
    std::unordered_set<Point> ust;

    REQUIRE(create_table(vec) != nullptr);
    REQUIRE(create_table(lst) != nullptr);
    REQUIRE(create_table(st) != nullptr);
    REQUIRE(create_table(ust) != nullptr);
}

TEST_CASE("create_table: provider type 2 (array containers)", "[create_table]") {
    std::vector<std::array<int, 5>> int_arrays;
    std::vector<std::array<std::string, 4>> str_arrays;
    std::vector<std::array<double, 3>> double_arrays;

    REQUIRE(create_table(int_arrays) != nullptr);
    REQUIRE(create_table(str_arrays) != nullptr);
    REQUIRE(create_table(double_arrays) != nullptr);
}

TEST_CASE("create_table: invalid types return nullptr", "[create_table]") {
    int a = 0;
    double b = 0.0;
    std::vector<std::vector<int>> nested;
    std::vector<std::array<std::vector<int>, 4>> invalid_arrays;

    REQUIRE(create_table(a) == nullptr);
    REQUIRE(create_table(b) == nullptr);
    REQUIRE(create_table(nested) == nullptr);
    REQUIRE(create_table(invalid_arrays) == nullptr);
}

TEST_CASE("create_table: returned table reflects changes in original", "[create_table]") {
    std::vector<std::array<double, 3>> data = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
    auto table = create_table(data);

    REQUIRE(table != nullptr);
    REQUIRE(table->row_count() == 2);
    REQUIRE(table->column_count() == 3);

    REQUIRE(table->set_value(0, 1, "42.5"));
    REQUIRE(data[0][1] == 42.5);
}