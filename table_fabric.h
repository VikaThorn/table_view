//
// Created by aurum on 18.08.2026.
//

#ifndef TABLE_VIEW_TABLE_FABRIC_H
#define TABLE_VIEW_TABLE_FABRIC_H

class Table;

namespace table_fabric {
    template <typename T>
    std::unique_ptr<Table> create_table(T& object);
}

#endif //TABLE_VIEW_TABLE_FABRIC_H
