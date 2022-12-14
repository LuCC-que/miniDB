#ifndef SQL_H
#define SQL_H

#include <string.h>

#include "../../utils/include/InputBuffer.h"
#include "table.h"

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_NEGATIVE_ID,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_STRING_TOO_LONG
} PrepareResult;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_DUPLICATE_KEY,
    EXECUTE_TABLE_FULL
} ExecuteResult;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT
} StatementType;

//====================== Statement======================
typedef struct {
    StatementType type;
    Row row_to_insert;  // for insertion
} Statement;

MetaCommandResult do_meta_command(const InputBuffer& input_buffer,
                                  Table& table);

PrepareResult prepare_statement(const InputBuffer& input_buffer,
                                Statement* statement);

PrepareResult prepare_insert(InputBuffer& input_buffer,
                             Statement* statement);

ExecuteResult execute_insert(Statement* statement,
                             Table& table);

ExecuteResult execute_select(Statement* statement,
                             Table& table);

ExecuteResult execute_statement(Statement* statement,
                                Table& table);

#endif