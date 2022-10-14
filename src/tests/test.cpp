// testing sql

#include <iostream>
#include <string>
#include <vector>

#include "InputBuffer.h"
#include "SQL.h"
// #include "table.h"

#define DB_FILE "./new.db"

void print_prompt() { printf("miniDB << "); }

void execute_a_command(InputBuffer input_buffer,
                       Table& table,
                       Statement* statement,
                       std::vector<std::string>& collector) {
    print_prompt();
    std::cout << input_buffer << std::endl;

    if (input_buffer.buffer[0] == '.') {
        switch (do_meta_command(input_buffer, table)) {
            case (META_COMMAND_SUCCESS):
                break;
            case (META_COMMAND_UNRECOGNIZED_COMMAND):
                OUTPUT
                    << "Unreconized command: "
                    << input_buffer
                    << std::endl;
                break;
        }
    }

    switch (prepare_statement(input_buffer, statement)) {
        case (PREPARE_SUCCESS):
            break;
        case (PREPARE_STRING_TOO_LONG):
            OUTPUT
                << "String is too long"
                << std::endl;
            break;
        case (PREPARE_NEGATIVE_ID):
            OUTPUT
                << "ID must be positive."
                << std::endl;
            break;
        case (PREPARE_UNRECOGNIZED_STATEMENT):
            OUTPUT
                << "Unrecognized command at start of: "
                << input_buffer
                << std::endl;
            break;
    }

    switch (execute_statement(statement, table)) {
        case (EXECUTE_SUCCESS):
            OUTPUT
                << "Executed."
                << std::endl;
            break;
        case (EXECUTE_TABLE_FULL):
            OUTPUT
                << "Error: Table full."
                << std::endl;
            break;
    }
}

void test_commands_executor(std::vector<std::string>& scripts, uint16_t repreat) {
    std::vector<std::string> collector;

    Table table(DB_FILE);
    Statement statement;

    while (repreat--) {
        for (std::string script : scripts) {
            InputBuffer ib(script);

            execute_a_command(ib, table, &statement, collector);
        }
    }

    // close_input_buffer(ib);
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
