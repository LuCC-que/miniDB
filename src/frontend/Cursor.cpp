#include "Cursor.h"

#include "table.h"
void Cursor::cursor_advance() {
    void* node = table->get_page(page_num);
    cell_num += 1;
    if (cell_num >= (*leaf_node_num_cells(node))) {
        end_of_table = true;
    }
}

void* Cursor::cursor_value() {
    void* page = table->get_page(page_num);

    return leaf_node_value(page, cell_num);
}

void leaf_node_insert(Cursor& cursor, uint32_t key, Row* value) {
    void* node = cursor.table->get_page(cursor.page_num);

    uint32_t num_cells = *leaf_node_num_cells(node);
    if (num_cells >= LEAF_NODE_MAX_CELLS) {
        // Node full
        printf("Need to implement splitting a leaf node.\n");
        exit(EXIT_FAILURE);
    }

    if (cursor.cell_num < num_cells) {
        // Make room for new cell
        for (uint32_t i = num_cells; i > cursor.cell_num; i--) {
            memcpy(leaf_node_cell(node, i),
                   leaf_node_cell(node, i - 1),
                   LEAF_NODE_CELL_SIZE);
        }
    }

    *(leaf_node_num_cells(node)) += 1;
    *(leaf_node_key(node, cursor.cell_num)) = key;
    serialize_row(value, leaf_node_value(node, cursor.cell_num));
}