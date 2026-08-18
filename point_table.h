//
// Табличное представление для контейнеров Point (поставщик 1 типа)
//

#ifndef TABLE_VIEW_POINT_TABLE_H
#define TABLE_VIEW_POINT_TABLE_H

#include <vector>
#include <ranges>
#include <iterator>

#include "concepts.h"
#include "point.h"
#include "table.h"

namespace table_fabric {

/// @brief Таблица для контейнеров Point.
/// Данные копируются во внутренний vector, оригинальный контейнер не изменяется.
class PointTable : public Table {
public:
    /// @brief Принимает любой контейнер Point и копирует его содержимое
    template <PointContainer Container>
    explicit PointTable(const Container& container) {
        std::ranges::copy(container, std::back_inserter(data_));
    }

    /// @brief Вывести в виде табличных данных в формате CSV
    /// Первой строкой в файле будут: name,x,y,z
    /// @param path Путь до файла, в который должно быть произведено сохранение
    /// @return Если успешно - true, в ином случае false
    bool save_to_csv(const std::filesystem::path& path) const override;

    /// @brief Получить количество строк
    size_t row_count() const override;

    /// @brief Получить количество столбцов
    size_t column_count() const override;

    /// @brief Заменить значение в указанной ячейке
    /// @param idx_row Номер строки
    /// @param idx_column Номер столбца
    /// @param value Новое значение в строковом представлении
    /// @return Если успешно - true, в ином случае false
    bool set_value(size_t idx_row, size_t idx_column, std::string value) override;

    /// @brief Добавить новую строку в таблицу
    /// @param new_row Новая линия таблицы, где каждый i-ый элемент является значением i-ого столбца
    /// @return Если успешно - true, в ином случае false
    bool add_row(std::vector<std::string> new_row) override;

    /// @brief Удалить строку из таблицы
    /// @param idx_row Индекс удаляемой строки
    /// @return Если успешно - true, в ином случае false
    bool remove_row(size_t idx_row) override;

private:
    std::vector<Point> data_;
};

}

#endif //TABLE_VIEW_POINT_TABLE_H