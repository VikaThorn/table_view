#ifndef TABLE_VIEW_POINT_H
#define TABLE_VIEW_POINT_H

#include <compare>
#include <cstddef>
#include <functional>
#include <string>

namespace table_fabric {

struct Point {
    std::string name;
    double x, y, z;
    
    /// @brief C++20: spaceship operator автоматически генерирует все операторы сравнения.
    auto operator<=>(const Point&) const = default;
};

}

namespace std {

template <>
struct hash<table_fabric::Point> {
    size_t operator()(const table_fabric::Point& p) const noexcept {
        const size_t h1 = std::hash<std::string>{}(p.name);
        const size_t h2 = std::hash<double>{}(p.x);
        const size_t h3 = std::hash<double>{}(p.y);
        const size_t h4 = std::hash<double>{}(p.z);

        size_t seed = h1;
        seed ^= h2 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h3 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= h4 + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

}

#endif //TABLE_VIEW_POINT_H