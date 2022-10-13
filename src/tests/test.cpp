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
    std::cout << "where you stack?\n";

    print_prompt();
    std::cout << "what is the statement?\n"
              << input_buffer->buffer
              << std::endl;

    if (input_buffer->buffer[0] == '.') {
        switch (do_meta_command(input_buffer)) {
            case (META_COMMAND_SUCCESS):
                break;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                printf("Unrecognized command '%s'\n", input_buffer->buffer);
                break;
        }
    }

    std::cout << "where" << std::endl;
    switch (prepare_statement(input_buffer, statement)) {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            printf("Unrecognized keyword at start of '%s'.\n",
                   input_buffer->buffer);
            break;
    }

    std::cout << "where 2" << std::endl;
    switch (execute_statement(statement, table)) {
        case (EXECUTE_SUCCESS):
            printf("Executed.\n");
            break;
        case (EXECUTE_TABLE_FULL):
            printf("Error: Table full.\n");
            break;
    }
}

int main() {
    std::vector<std::string> scripts{
        "insert 1 user1 person1@example.com",
        "select",
        ".exit",
    };

    std::vector<std::string> collector;

    Table* table = new_table();
    Statement statement;
    InputBuffer* ib = new_input_buffer();
    for (std::string& script : scripts) {
        ib->buffer = (char*)script.c_str();
        ib->buffer_length = script.length();
        ib->input_length = script.length();

        execute_a_command(ib, table, &statement, collector);
    }

    for (std::string& output : collector) {
        std::cout << output << std::endl;
    }

    close_input_buffer(ib);
    return 0;
}
