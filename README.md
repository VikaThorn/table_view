# Задача "Табличное представление"

## Входные данные
Существует несколько видов поставщиков данных.

### Поставщики первого типа
```c++
struct Point {
    std::string name;
    double x, y, z;
};

// Поставщиком является любой одномерный контейнер из C++20:
std::vector<Point> object_a0;
std::list<Point> object_a1;
std::set<Point> object_a2; // А такое можно? А как сделать, чтобы было можно?
std::unordered_set<Point> object_a3; // Аналогичные вопросы
```

### Поставщики второго типа
```c++
// Поставщик может иметь любой размер NxM c типом Type, где Type является либо строкой (std::string || std::string_view),
// либо арифметическим типом
std::vector<std::array<Type, N>> array_type
```

## Условие
Пускай существует следующий абстрактный класс (интерфейс) [`Table`](table.h).\
Тогда задача сводится к определению функции `create_table`, описанной в [`table_fabric.h`](table_fabric.h),
включая реализацию всех необходимых для этого классов и функций.\
Разрешена правка и коррекция представленного кода.\
Минимальное конечное решение должно быть представлено в виде CMake-проекта, сборка которого должна привести
к запускаемому файлу. Плюсом будет написание тестов.

## Примеры
```c++
std::vector<Point> object_a0;
auto table1 = create_table(object_a0); // table1 != nullptr
table1->add_row({"point_name", "10", "12.4", "1"}); // Вызов виртуального метода валиден

int a; // Не является типом поставщика
auto table2 = create_table(a); // table2 == nullptr

std::vector<std::array<std::string, 5>> array_type;
const auto table3 = create_table(array_type); // table3 != nullptr
const size_t row_count = table3->row_count(); // row_count == array_type.size()
```

## Структура проекта

- `table.h` — абстрактный интерфейс `Table`
- `table_fabric.h` — фабрика `create_table`
- `point.h` — структура `Point` и поддержка хеширования
- `concepts.h` — C++20 концепты для определения поставщиков
- `point_table.h` — реализация `Table` для контейнеров `Point`
- `array_table.h` — реализация `Table` для `std::vector<std::array<Type, N>>`
- `tests/` — тесты на Catch2
- `examples/main.cpp` — демонстрация работы фабрики

## Сборка и запуск

Проект использует CMake (версия 3.20+). Требуется компилятор с поддержкой C++20.

### Конфигурация и сборка

```bash
cmake -B build
cmake --build build --config Release
```

### Запуск примера

```bash
./build/examples/Release/example
```

### Запуск тестов

```bash
ctest --test-dir build -C Release --output-on-failure
```
Проект включает 28 тестов, покрывающих все компоненты: концепты, PointTable, ArrayTable и фабрику create_table.