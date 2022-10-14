#include "table.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

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
    }

    return pager.pages[page_num];
}

void Pager::pager_flush(uint32_t page_num, uint32_t size) {
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
        write(file_descriptor,
              pages[page_num],
              size);

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
