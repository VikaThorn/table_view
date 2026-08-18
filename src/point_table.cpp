#include "point_table.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace table_fabric {

namespace {

/// @brief Экранирование значения для CSV
std::string escape_csv(const std::string& value) {
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
std::string to_csv_string(double value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

}

bool PointTable::save_to_csv(const std::filesystem::path& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }

    file << "name,x,y,z\n";
    for (const auto& point : data_) {
        file << escape_csv(point.name) << ','
             << to_csv_string(point.x) << ','
             << to_csv_string(point.y) << ','
             << to_csv_string(point.z) << '\n';
    }

    return file.good();
}

size_t PointTable::row_count() const {
    return data_.size();
}

size_t PointTable::column_count() const {
    return 4; // name, x, y, z
}

bool PointTable::set_value(size_t idx_row, size_t idx_column, std::string value) {
    if (idx_row >= data_.size() || idx_column >= column_count()) {
        return false;
    }

    Point& point = data_[idx_row];

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

bool PointTable::add_row(std::vector<std::string> new_row) {
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

    data_.push_back(std::move(point));
    return true;
}

bool PointTable::remove_row(size_t idx_row) {
    if (idx_row >= data_.size()) {
        return false;
    }
    data_.erase(data_.begin() + static_cast<std::ptrdiff_t>(idx_row));
    return true;
}

}