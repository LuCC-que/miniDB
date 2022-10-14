#ifndef CURSOR_H
#define CURSOR_H

#include <iostream>

#include "table.h"

struct Cursor {
    Table* table;  // replace this by copy constructor
    uint32_t row_num;
    bool end_of_table;

    // copy table.. table_end
    Cursor(Table* t, const bool end)
        : table(t), row_num(t->num_rows), end_of_table(end) {
    }

    // table_start

    Cursor(Table* t)
        : table(t),
          row_num(0),
          end_of_table(t->num_rows == 0) {}

    void cursor_advance();

    void* cursor_value();
};

#endif
