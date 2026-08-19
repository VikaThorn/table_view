#include <array>
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include <point.h>
#include <table_fabric.h>

using table_fabric::Point;
using table_fabric::create_table;

int main() {
    std::vector<Point> object_a1;
    std::list<Point> object_a2;
    std::set<Point> object_a3;
    std::unordered_set<Point> object_a4;

    auto table1 = create_table(object_a1);
    auto table2 = create_table(object_a2);
    auto table3 = create_table(object_a3);
    auto table4 = create_table(object_a4);

    std::cout << "Table1 (vector<Point>):         " << (table1 ? "created" : "null") << "\n";
    std::cout << "Table2 (list<Point>):           " << (table2 ? "created" : "null") << "\n";
    std::cout << "Table3 (set<Point>):            " << (table3 ? "created" : "null") << "\n";
    std::cout << "Table4 (unordered_set<Point>):  " << (table4 ? "created" : "null") << "\n\n";

    if (table1) {
        table1->add_row({"point_name", "10", "12.4", "1"});
        std::cout << "Table1 rows after add_row:      " << table1->row_count() << "\n";
        std::cout << "Original vector size:           " << object_a1.size() << "\n\n";
    }

    std::vector<std::array<int, 5>> object_b1;
    std::vector<std::array<std::string, 4>> object_b2;
    std::vector<std::array<std::vector<int>, 4>> object_b3;

    auto table5 = create_table(object_b1);
    auto table6 = create_table(object_b2);
    auto table7 = create_table(object_b3);

    std::cout << "Table5 (array<int, 5>):         " << (table5 ? "created" : "null") << "\n";
    std::cout << "Table6 (array<string, 4>):      " << (table6 ? "created" : "null") << "\n";
    std::cout << "Table7 (array<vector<int>, 4>): " << (table7 ? "created" : "null") << "\n\n";

    if (table6) {
        table6->add_row({"Alice", "30", "Moscow", "Engineer"});
        table6->add_row({"Bob", "25", "SPb", "Developer"});
        table6->save_to_csv("example_output.csv");
        std::cout << "Table6 saved to example_output.csv\n\n";
    }

    int a = 0;
    double b = 0.0;
    std::vector<std::vector<int>> c;

    auto table8 = create_table(a);
    auto table9 = create_table(b);
    auto table10 = create_table(c);

    std::cout << "Table8 (int):                   " << (table8 ? "created" : "null") << "\n";
    std::cout << "Table9 (double):                " << (table9 ? "created" : "null") << "\n";
    std::cout << "Table10 (vector<vector<int>>):  " << (table10 ? "created" : "null") << "\n";

    return 0;
}