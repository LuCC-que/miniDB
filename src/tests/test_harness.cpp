#include "./include/test_harness.h"

#include "../frontend/include/SQL.h"
#include "../frontend/include/table.h"
#include "../utils/include/InputBuffer.h"

void print_prompt() { printf("miniDB << "); }

void execute_a_command(InputBuffer* input_buffer,
                       Table* table,
                       Statement* statement,
                       std::vector<std::string>& collector) {
    print_prompt();

    if (input_buffer->buffer[0] == '.') {
        switch (do_meta_command(input_buffer)) {
            case (META_COMMAND_SUCCESS):
                break;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                break;
        }
    }

    switch (prepare_statement(input_buffer, statement)) {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n",
                   input_buffer->buffer);
            break;
    }

    switch (execute_statement(statement, table)) {
        case (EXECUTE_SUCCESS):
            printf("Executed.\n");
            break;
        case (EXECUTE_TABLE_FULL):
            printf("Error: Table full.\n");
            break;
    }
}