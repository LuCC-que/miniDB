#include <iostream>
#include <string>
#include <vector>

#include "InputBuffer.h"
#include "SQL.h"

#define DB_FILE "./test1.db"

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

    // // test duplicate insertion, repeat 10 times
    std::vector<std::string> scripts2{
        "insert 1 user1 person1@example.com",
    };

    // test negative ID
    std::vector<std::string> scripts3{
        "insert -1 user1 person1@example.com",
    };

    // test insert
    std::vector<std::string> scripts4{
        "insert 18 user18 person18@example.com",
        "insert 1 user1 person1@example.com",
        "insert 7 user7 person7@example.com",
        "insert 10 user10 person10@example.com",
        "insert 29 user29 person29@example.com",
        "insert 23 user23 person23@example.com",
        "insert 4 user4 person4@example.com",
        "insert 14 user14 person14@example.com",
        "insert 30 user30 person30@example.com",
        "insert 15 user15 person15@example.com",
        "insert 26 user26 person26@example.com",
        "insert 22 user22 person22@example.com",
        "insert 19 user19 person19@example.com",
        "insert 2 user2 person2@example.com",
        "insert 21 user21 person21@example.com",
        "insert 11 user11 person11@example.com",
        "insert 6 user6 person6@example.com",
        "insert 20 user20 person20@example.com",
        "insert 5 user5 person5@example.com",
        "insert 8 user8 person8@example.com",
        "insert 9 user9 person9@example.com",
        "insert 3 user3 person3@example.com",
        "insert 12 user12 person12@example.com",
        "insert 27 user27 person27@example.com",
        "insert 17 user17 person17@example.com",
        "insert 16 user16 person16@example.com",
        "insert 13 user13 person13@example.com",
        "insert 24 user24 person24@example.com",
        "insert 25 user25 person25@example.com",
        "insert 28 user28 person28@example.com",
        ".btree",
        "select",
    };

    test_commands_executor(scripts1, 1);
    test_commands_executor(scripts2, 10);
    test_commands_executor(scripts3, 1);
    test_commands_executor(scripts4, 1);

    return 0;
}