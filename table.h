//
// Created by aurum on 18.08.2026.
//

#ifndef TABLE_VIEW_TABLE_H
#define TABLE_VIEW_TABLE_H

#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

class Table {
public:
    virtual ~Table() = default;
    /// @brief Вывести в виде табличных данных в формате CSV
    /// Первой строкой в файле должны быть логические имена колонок
    /// @param path Путь до файла, в который должно быть произведено сохранение
    /// @return Если успешно - true, в ином случае false
    virtual bool save_to_csv(const std::filesystem::path& path) const = 0;
    /// @brief Получить количество строк
    virtual size_t row_count() const = 0;
    /// @brief Получить количество столбцов
    virtual size_t column_count() const = 0;
    /// @brief Заменить значение в указанной ячейке
    /// @param idx_row Номер строки (отсчет с нуля)
    /// @param idx_column Номер столбца (отсчет с нуля)
    /// @param value Новое значение в строковом представлении
    /// @return Если успешно - true, в ином случае false
    virtual bool set_value(size_t idx_row, size_t idx_column, std::string value) = 0;
    /// @brief Добавить новую строку в таблицу
    /// @param new_row Новая линия таблицы, где каждый i-ый элемент является значением i-ого столбца
    /// @return Если успешно - true, в ином случае false
    virtual bool add_row(std::vector<std::string> new_row) = 0;
    /// @brief Удалить строку из таблицы
    /// @param idx_row Индекс удаляемой строки
    /// @return Если успешно - true, в ином случае false
    virtual bool remove_row(size_t idx_row) = 0;
};

#endif //TABLE_VIEW_TABLE_H
