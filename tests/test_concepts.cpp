#include <catch2/catch_test_macros.hpp>

#include <array>
#include <list>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include <concepts.h>
#include <point.h>

using table_fabric::Point;

static_assert(table_fabric::PointContainer<std::vector<Point>>);
static_assert(table_fabric::PointContainer<std::list<Point>>);
static_assert(table_fabric::PointContainer<std::set<Point>>);
static_assert(table_fabric::PointContainer<std::unordered_set<Point>>);

static_assert(!table_fabric::PointContainer<int>);
static_assert(!table_fabric::PointContainer<std::vector<int>>);

static_assert(table_fabric::ArrayTableProvider<std::vector<std::array<double, 3>>>);
static_assert(table_fabric::ArrayTableProvider<std::vector<std::array<std::string, 5>>>);
static_assert(table_fabric::ArrayTableProvider<std::vector<std::array<std::string_view, 4>>>);
static_assert(table_fabric::ArrayTableProvider<std::vector<std::array<int, 2>>>);

static_assert(!table_fabric::ArrayTableProvider<int>);
static_assert(!table_fabric::ArrayTableProvider<std::vector<int>>);
static_assert(!table_fabric::ArrayTableProvider<std::vector<std::array<std::vector<int>, 3>>>);

static_assert(table_fabric::TableProvider<std::vector<Point>>);
static_assert(table_fabric::TableProvider<std::vector<std::array<double, 3>>>);
static_assert(!table_fabric::TableProvider<int>);

TEST_CASE("concepts compile-time checks", "[concepts]") {
    REQUIRE(true);
}