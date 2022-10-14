#ifndef TABLE_H
#define TABLE_H
#include <iostream>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define TABLE_MAX_PAGES 100
// joey shortcut
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} Row;

typedef struct {
    int file_descriptor;
    uint32_t file_length;
    void* pages[TABLE_MAX_PAGES];
} Pager;

// testing row
const uint32_t ID_SIZE = size_of_attribute(Row, id);
const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);
const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);

// &ID_OFFSET &USERNAME_OFFSET &EMAIL_OFFSET
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;

const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
    uint32_t num_rows;
    // void* pages[TABLE_MAX_PAGES];
    Pager* pager;
} Table;

void serialize_row(Row* source, void* destination);

void deserialize_row(void* source, Row* destination);

void* row_slot(Table* table, uint32_t row_num);

Table* new_table();

void free_table(Table* table);

void pager_flush(Pager* pager, uint32_t page_num, uint32_t size);

Pager* pager_open(const char* filename);

void* get_page(Pager* pager, uint32_t page_num);

void print_row(Row* row);

void db_close(Table* table);

Table* db_open(const char* filename);
#endif