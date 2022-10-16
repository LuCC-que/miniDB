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
    // Cursor(Table* t, const bool end)
    //     : table(t), page_num(t->root_page_num), end_of_table(end) {
    //     void* root_node = t->get_page(table->root_page_num);
    //     uint32_t num_cells = *leaf_node_num_cells(root_node);
    //     cell_num = num_cells;
    // }

    // insert Constructor
    Cursor(Table* t, uint32_t key)
        : table(t) {
        void* root_node = table->get_page(table->root_page_num);

        if (get_node_type(root_node) == NODE_LEAF) {
            leaf_node_find(table->root_page_num, key);
        } else {
            // printf("Need to implement searching an internal node\n");
            // exit(EXIT_FAILURE);
            internal_node_find(table->root_page_num, key);
        }
    }

    // table_start

    Cursor(Table* t)
        : Cursor(t, 0) {
        void* node = table->get_page(page_num);
        uint32_t num_cells = *leaf_node_num_cells(node);
        end_of_table = (num_cells == 0);
    }

    void cursor_advance();
    void* cursor_value();

    void leaf_node_find(uint32_t p_num, uint32_t key);
    void leaf_node_insert(uint32_t key, Row* value);
    void leaf_node_split_and_insert(uint32_t key, Row* value);
    void create_new_root(uint32_t right_child_page_num);
    void internal_node_find(uint32_t p_num, uint32_t key);
    void internal_node_insert(uint32_t parent_page_num,
                              uint32_t child_page_num);
};

uint32_t* node_parent(void* node);

uint32_t get_unused_page_num(const Pager& pager);

void update_internal_node_key(void* node,
                              uint32_t old_key,
                              uint32_t new_key);

#endif
