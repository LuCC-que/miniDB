#include <stdlib.h>
#include <string.h>

#include "SQL.h"
#include "table.h"

MetaCommandResult do_meta_command(const InputBuffer& input_buffer, const Table& table) {
    if (input_buffer == ".exit") {
        table.~Table();
        exit(EXIT_SUCCESS);
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
    if (table.num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row* row_to_insert = &(statement->row_to_insert);

    serialize_row(row_to_insert, table.row_slot(table.num_rows));
    table.num_rows += 1;

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Statement* statement, Table& table) {
    Row row;
    for (uint32_t i = 0; i < table.num_rows; i++) {
        deserialize_row(table.row_slot(i), &row);
        print_row(&row);
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