#include "Cursor.h"

#include "table.h"
void Cursor::cursor_advance() {
    row_num += 1;
    if (row_num >= table->num_rows) {
        end_of_table = true;
    }
}

void* Cursor::cursor_value() {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = table->get_page(page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
}