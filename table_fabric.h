//
// Created by aurum on 18.08.2026.
//

#ifndef TABLE_VIEW_TABLE_FABRIC_H
#define TABLE_VIEW_TABLE_FABRIC_H

#include <memory>
#include <ranges>
#include <type_traits>

#include "array_table.h"
#include "concepts.h"
#include "point_table.h"
#include "table.h"

class Table;

namespace table_fabric {
    /// @brief Получить табличное представление из указанного объекта
    /// @param object Объект, из которого требуется создать табличное представление
    /// @tparam T Тип объекта, из которого требуется получить табличное представление
    /// @return Валидный указатель, если T соответствует типу хотя бы одного из поставщиков, иначе nullptr

    /// @brief Перегрузка для поставщиков 1 типа (контейнеры Point)
    template <PointContainer T>
    std::unique_ptr<Table> create_table(T& object) {
        return std::make_unique<PointTable<T>>(object);
    }

    /// @brief Перегрузка для поставщиков 2 типа (range из array<Type, N>)
    template <ArrayTableProvider T>
    std::unique_ptr<Table> create_table(T& object) {
        return std::make_unique<ArrayTable<T>>(object);
    }

    /// @brief Перегрузка для неподходящих типов (возвращает nullptr)
    template <typename T>
        requires (!TableProvider<T>)
    std::unique_ptr<Table> create_table(T&) {
        return nullptr;
    }
}

#endif //TABLE_VIEW_TABLE_FABRIC_H
