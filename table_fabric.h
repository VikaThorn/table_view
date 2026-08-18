//
// Created by aurum on 18.08.2026.
//

#ifndef TABLE_VIEW_TABLE_FABRIC_H
#define TABLE_VIEW_TABLE_FABRIC_H

#include <memory>

class Table;

namespace table_fabric {
    /// @brief Получить табличное представление из указанного объекта
    /// @param object Объект, из которого требуется создать табличное представление
    /// @tparam T Тип объекта, из которого требуется получить табличное представление
    /// @return Валидный указатель, если T соответствует типу хотя бы одного из поставщиков, иначе nullptr
    template <typename T>
    std::unique_ptr<Table> create_table(T& object);
}

#endif //TABLE_VIEW_TABLE_FABRIC_H
