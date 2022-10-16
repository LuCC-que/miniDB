#ifndef TABLE_H
#define TABLE_H

#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "../../backend/include/internalNode.h"

struct Pager {
    int file_descriptor;
    uint32_t file_length;
    void* pages[TABLE_MAX_PAGES];
    uint32_t num_pages;

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
        this->num_pages = file_length / PAGE_SIZE;

        if (file_length % PAGE_SIZE != 0) {
            printf("Db file is not a whole number of pages. Corrupt file.\n");
            exit(EXIT_FAILURE);
        }

        for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
            this->pages[i] = NULL;
        }
    }

    void pager_flush(
        uint32_t page_num);

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

struct Table {
    uint32_t root_page_num;
    Pager pager;

    Table(const char* filename)
        : pager(Pager(filename)), root_page_num(0) {
        if (pager.num_pages == 0) {
            // New database file. Initialize page 0 as leaf node.
            void* root_node = get_page(0);
            initialize_leaf_node(root_node);
            set_node_root(root_node, true);
        }
    }

    ~Table() {
        for (uint32_t i = 0; i < pager.num_pages; i++) {
            if (pager.pages[i] == NULL) {
                continue;
            }
            pager.pager_flush(i);
            free(pager.pages[i]);
            pager.pages[i] = NULL;
        }
    }

    void* get_page(uint32_t page_num);
};

void serialize_row(Row* source, void* destination);

void deserialize_row(void* source, Row* destination);

void print_row(Row* row);

void print_constants();

void indent(uint32_t level);

void print_tree(Table* table,
                uint32_t page_num,
                uint32_t indentation_level);

#endif