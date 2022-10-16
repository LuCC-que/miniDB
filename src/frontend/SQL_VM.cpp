#include <stdlib.h>
#include <string.h>

#include "Cursor.h"
#include "SQL.h"
#include "table.h"
MetaCommandResult do_meta_command(const InputBuffer& input_buffer,
                                  Table& table) {
    if (input_buffer == ".exit") {
        table.~Table();
        input_buffer.~InputBuffer();
        exit(EXIT_SUCCESS);

    } else if (input_buffer == ".btree") {
        std::cout << "Tree:" << std::endl;
        print_tree(&table, 0, 0);
        return META_COMMAND_SUCCESS;

    } else if (input_buffer == ".constants") {
        printf("Constants:\n");
        print_constants();
        return META_COMMAND_SUCCESS;

    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_insert(const InputBuffer& input_buffer, Statement* statement) {
    statement->type = STATEMENT_INSERT;

    char* temp = (char*)input_buffer.buffer.c_str();
    char* keyword = strtok(temp, " ");
    char* id_string = strtok(NULL, " ");
    char* username = strtok(NULL, " ");
    char* email = strtok(NULL, " ");

    if (id_string == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(id_string);

    if (id < 0) {
        return PREPARE_NEGATIVE_ID;
    }
    if (strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }
    if (strlen(email) > COLUMN_EMAIL_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    statement->row_to_insert.id = id;
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);

    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(const InputBuffer& input_buffer,
                                Statement* statement) {
    if (input_buffer == "insert") {
        // save the data to statement
        return prepare_insert(input_buffer, statement);
    }
    if (input_buffer == "select") {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_insert(Statement* statement, Table& table) {
    void* node = table.get_page(table.root_page_num);
    uint32_t num_cells = (*leaf_node_num_cells(node));

    Row* row_to_insert = &(statement->row_to_insert);
    uint32_t key_to_insert = row_to_insert->id;
    Cursor cursor(&table, key_to_insert);
    if (cursor.cell_num < num_cells) {
        uint32_t key_at_index = *leaf_node_key(node, cursor.cell_num);
        if (key_at_index == key_to_insert) {
            return EXECUTE_DUPLICATE_KEY;
        }
    }
    cursor.leaf_node_insert(row_to_insert->id, row_to_insert);
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement, Table& table) {
    Cursor start_cursor(&table);
    Row row;

    while (!(start_cursor.end_of_table)) {
        deserialize_row(start_cursor.cursor_value(), &row);
        print_row(&row);
        start_cursor.cursor_advance();
    }
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Statement* statement, Table& table) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            return execute_insert(statement, table);
        case (STATEMENT_SELECT):
            return execute_select(statement, table);
    }
}
