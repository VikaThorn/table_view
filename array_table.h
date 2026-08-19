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
#include <utility>
#include <tuple>
#include <string_view>

#include "concepts.h"
#include "table.h"

namespace table_fabric {

/// @brief Таблица для контейнеров std::array<Type, N> (поставщик 2 типа).
/// Хранит ссылку на оригинальный контейнер, все изменения отражаются в нём.
/// @tparam Container Тип контейнера (например, std::vector<std::array<double, 3>>)
template <ArrayTableProvider Container>
class ArrayTable : public Table {
public:
    /// @brief Создаёт вид на указанный контейнер
    /// @param container Ссылка на оригинальный контейнер
    explicit ArrayTable(Container& container) : container_(container) {}

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

        for (const auto& row : container_) {
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
        return container_.size();
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
        if (idx_row >= container_.size() || idx_column >= N) {
            return false;
        }

        auto it = container_.begin();
        std::advance(it, static_cast<std::ptrdiff_t>(idx_row));

        if constexpr (has_const_elements) {
            ArrayType original = *it;
            ArrayType modified = original;

            if (!modify_cell(modified[idx_column], value)) {
                return false;
            }

            container_.erase(it);
            auto [inserted_it, inserted] = container_.insert(std::move(modified));

            if (!inserted) {
                container_.insert(std::move(original));
                return false;
            }
            return true;
        } else {
            return modify_cell((*it)[idx_column], value);
        }
    }

    /// @brief Добавить новую строку в таблицу
    /// @param new_row Новая линия таблицы, где каждый i-ый элемент является значением i-ого столбца
    /// @return Если успешно - true, в ином случае false
    bool add_row(std::vector<std::string> new_row) override {
        if (new_row.size() != N) {
            return false;
        }

        ArrayType row{};
        for (std::size_t i = 0; i < N; ++i) {
            if (!modify_cell(row[i], new_row[i])) {
                return false;
            }
        }

        if constexpr (has_const_elements) {
            auto [inserted_it, inserted] = container_.insert(std::move(row));
            return inserted;
        } else {
            container_.push_back(std::move(row));
            return true;
        }
    }

    /// @brief Удалить строку из таблицы
    /// @param idx_row Индекс удаляемой строки
    /// @return Если успешно - true, в ином случае false
    bool remove_row(size_t idx_row) override {
        if (idx_row >= container_.size()) {
            return false;
        }
        auto it = container_.begin();
        std::advance(it, static_cast<std::ptrdiff_t>(idx_row));
        container_.erase(it);
        return true;
    }

private:
    Container& container_;

    /// @brief Тип элемента контейнера: std::array<Type, N>
    using ArrayType = std::remove_cvref_t<std::ranges::range_value_t<Container>>;
    /// @brief Тип ячейки массива
    using CellType = typename ArrayType::value_type;
    /// @brief Размер массива (количество столбцов)
    static constexpr std::size_t N = std::tuple_size_v<ArrayType>;

    /// @brief Определяет, являются ли элементы контейнера константными
    static constexpr bool has_const_elements =
        std::is_const_v<std::remove_reference_t<decltype(*std::declval<Container&>().begin())>>;

    /// @brief Парсит строку и присваивает значение ячейке
    static bool modify_cell(CellType& cell, const std::string& value) {
        if constexpr (std::is_same_v<std::remove_cvref_t<CellType>, std::string>) {
            cell = value;
            return true;
        } else if constexpr (std::is_same_v<std::remove_cvref_t<CellType>, std::string_view>) {
            return false;
        } else {
            return parse_arithmetic(cell, value);
        }
    }

    /// @brief Преобразование значения в строку для CSV
    static std::string to_csv_string(const CellType& value) {
        if constexpr (std::is_same_v<std::remove_cvref_t<CellType>, std::string>) {
            return escape_csv(value);
        } else if constexpr (std::is_same_v<std::remove_cvref_t<CellType>, std::string_view>) {
            return escape_csv(std::string(value));
        } else {
            std::ostringstream oss;
            oss << value;
            return oss.str();
        }
    }

    /// @brief Парсинг арифметического типа из строки
    template <typename T>
    requires std::is_arithmetic_v<T>
    static bool parse_arithmetic(T& target, const std::string& value) {
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
    static std::string escape_csv(const std::string& value) {
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