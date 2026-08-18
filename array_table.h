//
// Табличное представление для массивов (поставщик 2 типа)
//

#ifndef TABLE_VIEW_ARRAY_TABLE_H
#define TABLE_VIEW_ARRAY_TABLE_H

#include <array>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include "concepts.h"
#include "table.h"

namespace table_fabric {

/// @brief Таблица для контейнеров std::array<Type, N> (поставщик 2 типа).
/// Данные копируются во внутренний vector, оригинальный контейнер не изменяется.
/// @tparam Type Тип ячейки (строка или арифметический тип)
/// @tparam N Размер массива (количество столбцов)
template <AllowedCellType Type, std::size_t N>
class ArrayTable : public Table {
public:
    /// @brief Принимает любой range из std::array<Type, N> и копирует его содержимое
    template <typename Container>
    requires std::ranges::range<Container>
          && std::same_as<std::remove_cvref_t<std::ranges::range_value_t<Container>>, std::array<Type, N>>
    explicit ArrayTable(const Container& container) {
        std::ranges::copy(container, std::back_inserter(data_));
    }

    /// @brief Вывести в виде табличных данных в формате CSV
    /// Первой строкой в файле будут: col_0,col_1,...,col_N-1
    /// @param path Путь до файла, в который должно быть произведено сохранение
    /// @return Если успешно - true, в ином случае false
    bool save_to_csv(const std::filesystem::path& path) const override {
        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }

        for (std::size_t i = 0; i < N; ++i) {
            file << "col_" << i;
            if (i + 1 < N) {
                file << ',';
            }
        }
        file << '\n';

        for (const auto& row : data_) {
            for (std::size_t i = 0; i < N; ++i) {
                file << to_csv_string(row[i]);
                if (i + 1 < N) {
                    file << ',';
                }
            }
            file << '\n';
        }

        return file.good();
    }

    /// @brief Получить количество строк
    size_t row_count() const override {
        return data_.size();
    }

    /// @brief Получить количество столбцов
    size_t column_count() const override {
        return N;
    }

    /// @brief Заменить значение в указанной ячейке
    /// @param idx_row Номер строки
    /// @param idx_column Номер столбца
    /// @param value Новое значение в строковом представлении
    /// @return Если успешно - true, в ином случае false
    bool set_value(size_t idx_row, size_t idx_column, std::string value) override {
        if (idx_row >= data_.size() || idx_column >= N) {
            return false;
        }

        return parse_and_assign(data_[idx_row][idx_column], value);
    }

    /// @brief Добавить новую строку в таблицу
    /// @param new_row Новая линия таблицы, где каждый i-ый элемент является значением i-ого столбца
    /// @return Если успешно - true, в ином случае false
    bool add_row(std::vector<std::string> new_row) override {
        if (new_row.size() != N) {
            return false;
        }

        std::array<Type, N> row;
        for (std::size_t i = 0; i < N; ++i) {
            if (!parse_and_assign(row[i], new_row[i])) {
                return false;
            }
        }

        data_.push_back(std::move(row));
        return true;
    }

    /// @brief Удалить строку из таблицы
    /// @param idx_row Индекс удаляемой строки
    /// @return Если успешно - true, в ином случае false
    bool remove_row(size_t idx_row) override {
        if (idx_row >= data_.size()) {
            return false;
        }
        data_.erase(data_.begin() + static_cast<std::ptrdiff_t>(idx_row));
        return true;
    }

private:
    std::vector<std::array<Type, N>> data_;

    /// @brief Преобразование значения в строку для CSV
    std::string to_csv_string(const Type& value) const {
        if constexpr (std::is_same_v<std::remove_cvref_t<Type>, std::string>) {
            return escape_csv(value);
        } else if constexpr (std::is_same_v<std::remove_cvref_t<Type>, std::string_view>) {
            return escape_csv(std::string(value));
        } else {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
    }

    /// @brief Парсинг строки и присваивание значению
    bool parse_and_assign(Type& target, const std::string& value) const {
        if constexpr (std::is_same_v<std::remove_cvref_t<Type>, std::string>) {
            target = value;
            return true;
        } else if constexpr (std::is_same_v<std::remove_cvref_t<Type>, std::string_view>) {
            return false;
        } else {
            return parse_arithmetic(target, value);
        }
    }

    /// @brief Парсинг арифметического типа из строки
    template <typename T>
    requires std::is_arithmetic_v<T>
    bool parse_arithmetic(T& target, const std::string& value) const {
        if constexpr (std::is_integral_v<T>) {
            T result;
            auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), result);
            if (ec == std::errc() && ptr == value.data() + value.size()) {
                target = result;
                return true;
            }
            return false;
        } else {
            try {
                if constexpr (std::is_same_v<T, float>) {
                    target = std::stof(value);
                } else if constexpr (std::is_same_v<T, double>) {
                    target = std::stod(value);
                } else if constexpr (std::is_same_v<T, long double>) {
                    target = std::stold(value);
                } else {
                    target = static_cast<T>(std::stod(value));
                }
                return true;
            } catch (...) {
                return false;
            }
        }
    }

    /// @brief Экранирование значения для CSV
    std::string escape_csv(const std::string& value) const {
        if (value.find_first_of(",\"\n\r") == std::string::npos) {
            return value;
        }
        std::string escaped = "\"";
        for (char c : value) {
            if (c == '"') {
                escaped += "\"\"";
            } else {
                escaped += c;
            }
        }
        escaped += "\"";
        return escaped;
    }
};

}

#endif //TABLE_VIEW_ARRAY_TABLE_H