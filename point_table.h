//
// Табличное представление для контейнеров Point (поставщик 1 типа)
//

#ifndef TABLE_VIEW_POINT_TABLE_H
#define TABLE_VIEW_POINT_TABLE_H

#include <vector>
#include <ranges>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <cstddef>
#include <filesystem>
#include <fstream>

#include "concepts.h"
#include "point.h"
#include "table.h"

namespace table_fabric {

/// @brief Таблица для контейнеров Point.
/// Хранит ссылку на оригинальный контейнер, все изменения отражаются в нём.
/// @tparam Container Тип контейнера Point
template <PointContainer Container>
class PointTable : public Table {
public:
    /// @brief Создаёт вид на указанный контейнер
    /// @param container Ссылка на оригинальный контейнер
    explicit PointTable(Container& container) : container_(container) {}

    /// @brief Вывести в виде табличных данных в формате CSV
    /// Первой строкой в файле будут: name,x,y,z
    /// @param path Путь до файла, в который должно быть произведено сохранение
    /// @return Если успешно - true, в ином случае false
    bool save_to_csv(const std::filesystem::path& path) const override {
        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }

        file << "name,x,y,z\n";
        for (const auto& point : container_) {
            file << escape_csv(point.name) << ','
                 << to_csv_string(point.x) << ','
                 << to_csv_string(point.y) << ','
                 << to_csv_string(point.z) << '\n';
        }

        return file.good();
    }

    /// @brief Получить количество строк
    size_t row_count() const override {
        return container_.size();
    }

    /// @brief Получить количество столбцов
    size_t column_count() const override {
        return 4; // name, x, y, z
    }

    /// @brief Заменить значение в указанной ячейке
    /// @param idx_row Номер строки
    /// @param idx_column Номер столбца
    /// @param value Новое значение в строковом представлении
    /// @return Если успешно - true, в ином случае false
    bool set_value(size_t idx_row, size_t idx_column, std::string value) override {
        if (idx_row >= container_.size() || idx_column >= column_count()) {
            return false;
        }

        auto it = container_.begin();
        std::advance(it, static_cast<std::ptrdiff_t>(idx_row));

        if constexpr (has_const_elements) {
            Point original = *it;
            Point modified = original;

            if (!modify_point(modified, idx_column, std::move(value))) {
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
            return modify_point(*it, idx_column, std::move(value));
        }
    }

    /// @brief Добавить новую строку в таблицу
    /// @param new_row Новая линия таблицы, где каждый i-ый элемент является значением i-ого столбца
    /// @return Если успешно - true, в ином случае false
    bool add_row(std::vector<std::string> new_row) override {
        if (new_row.size() != column_count()) {
            return false;
        }

        Point point;
        point.name = std::move(new_row[0]);

        try {
            point.x = std::stod(new_row[1]);
            point.y = std::stod(new_row[2]);
            point.z = std::stod(new_row[3]);
        } catch (const std::exception&) {
            return false;
        }

        if constexpr (has_const_elements) {
            auto [inserted_it, inserted] = container_.insert(std::move(point));
            return inserted;
        } else {
            container_.push_back(std::move(point));
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

    /// @brief Определяет, являются ли элементы контейнера константными.
    /// Для set/unordered_set разыменование итератора даёт const Point&,
    /// для vector/list — Point&.
    static constexpr bool has_const_elements =
        std::is_const_v<std::remove_reference_t<decltype(*std::declval<Container&>().begin())>>;

    /// @brief Изменяет указанное поле точки
    static bool modify_point(Point& point, size_t idx_column, std::string value) {
        if (idx_column == 0) {
            point.name = std::move(value);
            return true;
        }

        try {
            const double parsed = std::stod(value);
            switch (idx_column) {
                case 1: point.x = parsed; break;
                case 2: point.y = parsed; break;
                case 3: point.z = parsed; break;
                default: return false;
            }
            return true;
        } catch (const std::exception&) {
            return false;
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

    /// @brief Преобразование double в строку без лишних нулей
    static std::string to_csv_string(double value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }
};

}

#endif //TABLE_VIEW_POINT_TABLE_H