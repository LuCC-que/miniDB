#include "table.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "../../backend/include/internalNode.h"
#include "../../backend/include/leafNode.h"

void serialize_row(Row* source, void* destination) {
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void* source, Row* destination) {
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void* Table::get_page(uint32_t page_num) {
    if (page_num > TABLE_MAX_PAGES) {
        std::cout
            << "Tried to fetch page number out of bounds: "
            << page_num
            << " > "
            << TABLE_MAX_PAGES
            << std::endl;

        exit(EXIT_FAILURE);
    }

    if (pager.pages[page_num] == NULL) {
        // Cache miss. Allocate memory and load from file.
        void* page = malloc(PAGE_SIZE);
        uint32_t num_pages = pager.file_length / PAGE_SIZE;

        // We might save a partial page at the end of the file
        if (pager.file_length % PAGE_SIZE) {
            num_pages += 1;
        }

        if (page_num <= num_pages) {
            lseek(pager.file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
            ssize_t bytes_read = read(pager.file_descriptor, page, PAGE_SIZE);
            if (bytes_read == -1) {
                std::cout
                    << "Error reading file: "
                    << errno
                    << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        pager.pages[page_num] = page;

        if (page_num >= pager.num_pages) {
            pager.num_pages = page_num + 1;
        }
    }

    return pager.pages[page_num];
}

void Pager::pager_flush(uint32_t page_num) {
    if (pages[page_num] == NULL) {
        std::cout
            << "Tried to flush null page"
            << std::endl;

        exit(EXIT_FAILURE);
    }

    off_t offset = lseek(file_descriptor, page_num * PAGE_SIZE, SEEK_SET);

    if (offset == -1) {
        std::cout
            << "Error seeking: "
            << errno
            << std::endl;
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written =
        write(file_descriptor, pages[page_num], PAGE_SIZE);

    std::cout << "miniDB >> writed into db" << std::endl;

    if (bytes_written == -1) {
        std::cout
            << "Error writing: "
            << errno
            << std::endl;
        exit(EXIT_FAILURE);
    }
}

void print_row(Row* row) {
    std::cout
        << "miniDB >> ("
        << row->id
        << " "
        << row->username
        << " "
        << row->email
        << ")"
        << std::endl;
}

void print_constants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void indent(uint32_t level) {
    for (uint32_t i = 0; i < level; i++) {
        printf("  ");
    }
}

void print_tree(Table* table, uint32_t page_num, uint32_t indentation_level) {
    void* node = table->get_page(page_num);
    uint32_t num_keys, child;

    switch (get_node_type(node)) {
        case (NODE_LEAF):
            num_keys = *leaf_node_num_cells(node);
            indent(indentation_level);
            printf("- leaf (size %d)\n", num_keys);
            for (uint32_t i = 0; i < num_keys; i++) {
                indent(indentation_level + 1);
                printf("- %d\n", *leaf_node_key(node, i));
            }
            break;
        case (NODE_INTERNAL):
            num_keys = *internal_node_num_keys(node);
            indent(indentation_level);
            printf("- internal (size %d)\n", num_keys);
            for (uint32_t i = 0; i < num_keys; i++) {
                child = *internal_node_child(node, i);
                print_tree(table, child, indentation_level + 1);

                indent(indentation_level + 1);
                printf("- key %d\n", *internal_node_key(node, i));
            }
            child = *internal_node_right_child(node);
            print_tree(table, child, indentation_level + 1);
            break;
    }
}