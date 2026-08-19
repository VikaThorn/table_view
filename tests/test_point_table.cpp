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
    PointTable<std::vector<Point>> table(points);

    REQUIRE(table.row_count() == 2);
    REQUIRE(table.column_count() == 4);
}

TEST_CASE("PointTable: view reflects changes in original vector", "[point_table]") {
    std::vector<Point> points = {{"a", 1.0, 2.0, 3.0}};
    PointTable<std::vector<Point>> table(points);

    REQUIRE(table.set_value(0, 1, "42.5"));
    REQUIRE(points[0].x == 42.5);
}

TEST_CASE("PointTable: view reflects changes in original set", "[point_table]") {
    std::set<Point> points = {{"a", 1.0, 2.0, 3.0}};
    PointTable<std::set<Point>> table(points);

    REQUIRE(table.set_value(0, 1, "42.5"));
    REQUIRE(points.size() == 1);
    REQUIRE(points.begin()->x == 42.5);
}

TEST_CASE("PointTable: view reflects changes in original unordered_set", "[point_table]") {
    std::unordered_set<Point> points = {{"a", 1.0, 2.0, 3.0}};
    PointTable<std::unordered_set<Point>> table(points);

    REQUIRE(table.set_value(0, 1, "42.5"));
    REQUIRE(points.size() == 1);
    REQUIRE(points.begin()->x == 42.5);
}

TEST_CASE("PointTable: view works with list", "[point_table]") {
    std::list<Point> points = {{"a", 1.0, 2.0, 3.0}, {"b", 4.0, 5.0, 6.0}};
    PointTable<std::list<Point>> table(points);

    REQUIRE(table.row_count() == 2);
    REQUIRE(table.set_value(0, 1, "99.9"));
    REQUIRE(points.front().x == 99.9);
}

TEST_CASE("PointTable: add_row reflects in original vector", "[point_table]") {
    std::vector<Point> points;
    PointTable<std::vector<Point>> table(points);

    REQUIRE(table.add_row({"point_name", "10", "12.4", "1"}));
    REQUIRE(points.size() == 1);
    REQUIRE(points[0].name == "point_name");
    REQUIRE(points[0].x == 10);
}

TEST_CASE("PointTable: add_row reflects in original list", "[point_table]") {
    std::list<Point> points;
    PointTable<std::list<Point>> table(points);

    REQUIRE(table.add_row({"new_point", "5", "6", "7"}));
    REQUIRE(points.size() == 1);
    REQUIRE(points.front().name == "new_point");
}

TEST_CASE("PointTable: add_row to set with duplicate returns false", "[point_table]") {
    std::set<Point> points = {{"a", 1, 2, 3}};
    PointTable<std::set<Point>> table(points);

    REQUIRE(!table.add_row({"a", "1", "2", "3"}));
    REQUIRE(points.size() == 1);
}

TEST_CASE("PointTable: add_row to set with new element works", "[point_table]") {
    std::set<Point> points = {{"a", 1, 2, 3}};
    PointTable<std::set<Point>> table(points);

    REQUIRE(table.add_row({"b", "4", "5", "6"}));
    REQUIRE(points.size() == 2);
}

TEST_CASE("PointTable: remove_row reflects in original vector", "[point_table]") {
    std::vector<Point> points = {{"a", 1, 2, 3}, {"b", 4, 5, 6}};
    PointTable<std::vector<Point>> table(points);

    REQUIRE(table.remove_row(0));
    REQUIRE(points.size() == 1);
    REQUIRE(points[0].name == "b");
}

TEST_CASE("PointTable: remove_row reflects in original list", "[point_table]") {
    std::list<Point> points = {{"a", 1, 2, 3}, {"b", 4, 5, 6}};
    PointTable<std::list<Point>> table(points);

    REQUIRE(table.remove_row(0));
    REQUIRE(points.size() == 1);
    REQUIRE(points.front().name == "b");
}

TEST_CASE("PointTable: set_value with invalid number leaves original unchanged", "[point_table]") {
    std::vector<Point> points = {{"a", 1.0, 2.0, 3.0}};
    PointTable<std::vector<Point>> table(points);

    REQUIRE(!table.set_value(0, 1, "abc"));
    REQUIRE(points[0].x == 1.0);
}

TEST_CASE("PointTable: save_to_csv", "[point_table]") {
    std::vector<Point> points = {{"a", 1.0, 2.0, 3.0}};
    PointTable<std::vector<Point>> table(points);

    REQUIRE(table.save_to_csv("test_point_output.csv"));
}