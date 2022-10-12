#ifndef InputBuffer_h
#define InputBuffer_h

#include <stdio.h>

typedef struct {
    char* buffer;
    size_t buffer_length;
    ssize_t input_length;
} InputBuffer;

// Empty constructor
InputBuffer* new_input_buffer();

// string constructor
void read_input(InputBuffer* input_buffer);

// destructor
void close_input_buffer(InputBuffer* input_buffer);

#endif