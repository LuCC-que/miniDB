// testing sql

#include <iostream>
#include <string>
#include <vector>

#include "InputBuffer.h"
#include "SQL.h"
#include "table.h"

void print_prompt() { printf("miniDB << "); }

void execute_a_command(InputBuffer* input_buffer,
                       Table* table,
                       Statement* statement,
                       std::vector<std::string>& collector) {
    print_prompt();
    std::cout << input_buffer->buffer << "sad" << std::endl;

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
        case (PREPARE_STRING_TOO_LONG):
            printf("miniDB >> String is too long.\n");
            break;
        case (PREPARE_NEGATIVE_ID):
            printf("miniDB >> ID must be positive.\n");
            break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("miniDB >> Unrecognized keyword at start of '%s'.\n",
                   input_buffer->buffer);
            break;
    }

    switch (execute_statement(statement, table)) {
        case (EXECUTE_SUCCESS):
            break;

        case (EXECUTE_TABLE_FULL):
            printf("miniDB >> Error: Table full.\n");
            break;
    }
}

void test_commands_executor(std::vector<std::string>& scripts, uint16_t repreat) {
    std::vector<std::string> collector;

    Table* table = new_table();
    Statement statement;
    InputBuffer* ib = new_input_buffer();

    while (repreat--) {
        for (std::string script : scripts) {
            ib->buffer = (char*)script.c_str();
            ib->buffer_length = script.length();
            ib->input_length = script.length();

            execute_a_command(ib, table, &statement, collector);
        }
    }

    // close_input_buffer(ib);
    free_table(table);
}

int main() {
    // test 1 insertion

    std::vector<std::string> scripts1{
        "insert 1 user1 person1@example.com",
        "select",
    };

    // // test 1000 insertion
    std::vector<std::string> scripts2{
        "insert 1 user1 person1@example.com",
    };

    // test negative ID
    std::vector<std::string> scripts3{
        "insert -1 user1 person1@example.com",
    };

    test_commands_executor(scripts1, 1);
    test_commands_executor(scripts2, 10);
    test_commands_executor(scripts3, 1);

    return 0;
}
