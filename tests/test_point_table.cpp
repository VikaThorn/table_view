#include <catch2/catch_test_macros.hpp>

#include <list>
#include <set>
#include <unordered_set>
#include <vector>

#include <point.h>
#include <point_table.h>

using table_fabric::Point;
using table_fabric::PointTable;

TEST_CASE("PointTable: basic counts from vector", "[point_table]") {
    std::vector<Point> points = {
        {"a", 1.0, 2.0, 3.0},
        {"b", 4.0, 5.0, 6.0},
    };
    PointTable table(points);

    REQUIRE(table.row_count() == 2);
    REQUIRE(table.column_count() == 4);
}

TEST_CASE("PointTable: constructed from set and unordered_set", "[point_table]") {
    std::set<Point> set_points = {{"a", 1, 2, 3}, {"b", 4, 5, 6}};
    PointTable from_set(set_points);
    REQUIRE(from_set.row_count() == 2);

    std::unordered_set<Point> unordered_points = {{"a", 1, 2, 3}};
    PointTable from_unordered(unordered_points);
    REQUIRE(from_unordered.row_count() == 1);
}

TEST_CASE("PointTable: add_row valid and invalid", "[point_table]") {
    std::vector<Point> points;
    PointTable table(points);

    REQUIRE(table.add_row({"point_name", "10", "12.4", "1"}));
    REQUIRE(table.row_count() == 1);

    REQUIRE(!table.add_row({"x", "not_a_number", "1", "1"}));
    REQUIRE(table.row_count() == 1);

    REQUIRE(!table.add_row({"a", "1"}));
    REQUIRE(table.row_count() == 1);
}

TEST_CASE("PointTable: set_value", "[point_table]") {
    std::vector<Point> points = {{"a", 1.0, 2.0, 3.0}};
    PointTable table(points);

    REQUIRE(table.set_value(0, 0, "new_name"));
    REQUIRE(table.set_value(0, 1, "42.5"));

    REQUIRE(!table.set_value(5, 0, "x"));
    REQUIRE(!table.set_value(0, 10, "x"));

    REQUIRE(!table.set_value(0, 1, "abc"));
}

TEST_CASE("PointTable: remove_row", "[point_table]") {
    std::vector<Point> points = {{"a", 1, 2, 3}, {"b", 4, 5, 6}};
    PointTable table(points);

    REQUIRE(table.remove_row(0));
    REQUIRE(table.row_count() == 1);
    REQUIRE(!table.remove_row(10));
}

TEST_CASE("PointTable: save_to_csv", "[point_table]") {
    std::vector<Point> points = {{"a", 1.0, 2.0, 3.0}};
    PointTable table(points);

    REQUIRE(table.save_to_csv("test_point_output.csv"));
}