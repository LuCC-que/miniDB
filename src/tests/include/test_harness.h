#ifndef TEST_HARNESS_H
#define TEST_HARNESS_H

#include <iostream>
#include <string>
#include <vector>

void print_prompt();

void execute_a_command(InputBuffer* input_buffer,
                       Table* table,
                       Statement* statement,
                       std::vector<std::string>& collector);

#endif