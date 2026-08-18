#ifndef TABLE_VIEW_CONCEPTS_H
#define TABLE_VIEW_CONCEPTS_H

#include <array>
#include <concepts>
#include <cstddef>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>

#include "point.h"

namespace table_fabric {

/// @brief Определяет, является ли тип std::array<Type, N>
template <typename T>
struct is_std_array : std::false_type {};

template <typename Type, std::size_t N>
struct is_std_array<std::array<Type, N>> : std::true_type {};

template <typename T>
inline constexpr bool is_std_array_v = is_std_array<std::remove_cvref_t<T>>::value;


/// @brief Допустимый тип ячейки: строка или арифметический тип
template <typename T>
concept AllowedCellType =
    std::same_as<std::remove_cvref_t<T>, std::string>
    || std::same_as<std::remove_cvref_t<T>, std::string_view>
    || std::is_arithmetic_v<std::remove_cvref_t<T>>;

/// @brief Тип 1: одномерный range из Point
template <typename T>
concept PointContainer =
    std::ranges::range<T>
    && std::same_as<std::remove_cvref_t<std::ranges::range_value_t<T>>, Point>;

/// @brief Тип 2: range из std::array<Type, N> с допустимым Type
template <typename T>
concept ArrayTableProvider =
    std::ranges::range<T>
    && is_std_array_v<std::ranges::range_value_t<T>>
    && AllowedCellType<typename std::remove_cvref_t<std::ranges::range_value_t<T>>::value_type>;

/// @brief Любой поставщик данных (1-го или 2-го типа)
template <typename T>
concept TableProvider = PointContainer<T> || ArrayTableProvider<T>;

}

#endif //TABLE_VIEW_CONCEPTS_H