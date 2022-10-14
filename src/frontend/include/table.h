#ifndef TABLE_H
#define TABLE_H
#include <fcntl.h>
#include <unistd.h>

#include <cstring>
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

struct Pager {
    int file_descriptor;
    uint32_t file_length;
    void* pages[TABLE_MAX_PAGES];

    Pager() {}

    Pager(const char* filename) {
        int fd = open(filename,
                      O_RDWR |      // Read/Write mode
                          O_CREAT,  // Create file if it does not exist
                      S_IWUSR |     // User write permission
                          S_IRUSR   // User read permission
        );

        if (fd == -1) {
            printf("Unable to open file\n");
            exit(EXIT_FAILURE);
        }

        off_t file_length = lseek(fd,
                                  0,
                                  SEEK_END);

        this->file_descriptor = fd;
        this->file_length = file_length;

        for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
            this->pages[i] = NULL;
        }
    }

    void pager_flush(
        uint32_t page_num,
        uint32_t size);

    ~Pager() {
        std::cout << "cleaning the pager" << std::endl;
        int result = close(this->file_descriptor);
        if (result == -1) {
            printf("Error closing db file.\n");
            exit(EXIT_FAILURE);
        }

        // last save free
        for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
            void* page = this->pages[i];
            if (page) {
                free(page);
                this->pages[i] = NULL;
            }
        }
    }
};

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

struct Table {
    uint32_t num_rows;
    Pager pager;

    Table(const char* filename)
        : pager(Pager(filename)) {
        // after the initialization of pager
        num_rows = pager.file_length / ROW_SIZE;
    }

    ~Table() {
        uint32_t num_full_pages = num_rows / ROWS_PER_PAGE;

        for (uint32_t i = 0; i < num_full_pages; i++) {
            if (pager.pages[i] == NULL) {
                continue;
            }
            pager.pager_flush(i, PAGE_SIZE);
            free(pager.pages[i]);
            pager.pages[i] = NULL;
        }

        uint32_t num_additional_rows = num_rows % ROWS_PER_PAGE;
        if (num_additional_rows > 0) {
            uint32_t page_num = num_full_pages;
            if (pager.pages[page_num] != NULL) {
                pager.pager_flush(page_num,
                                  num_additional_rows * ROW_SIZE);
                free(pager.pages[page_num]);
                pager.pages[page_num] = NULL;
            }
        }
    }

    void* get_page(uint32_t page_num);
};

void serialize_row(Row* source, void* destination);

void deserialize_row(void* source, Row* destination);

void print_row(Row* row);

#endif