#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "InputBuffer.h"
#include "SQL.h"
#include "table.h"

void print_prompt() { printf("miniDB << "); }

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    Table table(filename);
    while (true) {
        PROMPT;
        InputBuffer input_buffer{};
        input_buffer.read_input();
        // std::cout << input_buffer << std::endl;

        if (input_buffer.buffer[0] == '.') {
            switch (do_meta_command(input_buffer, table)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    OUTPUT
                        << "Unreconized command: "
                        << input_buffer
                        << std::endl;
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_STRING_TOO_LONG):
                OUTPUT
                    << "String is too long"
                    << std::endl;
                continue;
            case (PREPARE_NEGATIVE_ID):
                OUTPUT
                    << "ID must be positive."
                    << std::endl;
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                OUTPUT
                    << "Unrecognized command at start of: "
                    << input_buffer
                    << std::endl;
                continue;
        }

        switch (execute_statement(&statement, table)) {
            case (EXECUTE_SUCCESS):
                OUTPUT
                    << "Executed."
                    << std::endl;
                break;
            case (EXECUTE_DUPLICATE_KEY):
                printf("Error: Duplicate key.\n");
                break;
            case (EXECUTE_TABLE_FULL):
                OUTPUT
                    << "Error: Table full."
                    << std::endl;
                break;
        }
    }
}
