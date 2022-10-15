#ifndef CURSOR_H
#define CURSOR_H

#include <iostream>

#include "table.h"

struct Cursor {
    Table* table;  // replace this by copy constructor
    uint32_t page_num;
    uint32_t cell_num;
    bool end_of_table;

    // copy table.. table_end
    Cursor(Table* t, const bool end)
        : table(t), page_num(t->root_page_num), end_of_table(end) {
        void* root_node = t->get_page(table->root_page_num);
        uint32_t num_cells = *leaf_node_num_cells(root_node);
        cell_num = num_cells;
    }

    // table_start

    Cursor(Table* t)
        : table(t),
          page_num(t->root_page_num),
          cell_num(0) {
        void* root_node = t->get_page(table->root_page_num);
        uint32_t num_cells = *leaf_node_num_cells(root_node);
        end_of_table = (num_cells == 0);
    }

    void cursor_advance();

    void* cursor_value();
};

void leaf_node_insert(Cursor& cursor, uint32_t key, Row* value);

#endif
