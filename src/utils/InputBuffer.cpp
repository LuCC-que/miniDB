#include "InputBuffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <iostream>
// empty constructor
// InputBuffer* new_input_buffer() {
//     InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
//     input_buffer->buffer = NULL;
//     input_buffer->buffer_length = 0;
//     input_buffer->input_length = 0;

//     return input_buffer;
// }

char asciitolower(char in) {
    if (in <= 'Z' && in >= 'A')
        return in - ('Z' - 'z');
    return in;
}

// string constructor
void InputBuffer::read_input() {
    std::string temp;
    std::getline(std::cin, temp);

    if (temp.length() < 1) {
        std::cout
            << "failed to read the input"
            << std::endl;

        exit(EXIT_FAILURE);
    }

    std::transform(temp.begin(), temp.end(), temp.begin(), asciitolower);
    this->buffer = temp;
    this->buffer_length = buffer.length();
    this->input_length = buffer_length;
}

bool InputBuffer::_max_match(const std::string& s) const {
    std::string::const_iterator c_it1 = this->buffer.begin();
    std::string::const_iterator c_it2 = s.begin();

    while (c_it2 != s.end()) {
        if (c_it1 == this->buffer.end() ||
            *c_it1 != *c_it2) {
            return false;
        }
        c_it1++;
        c_it2++;
    }

    return true;
}

// destructor
// void close_input_buffer(InputBuffer* input_buffer) {
//     free(input_buffer->buffer);
//     free(input_buffer);
// }