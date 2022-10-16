#ifndef InputBuffer_h
#define InputBuffer_h

#include <stdio.h>

#include <iostream>
#include <string>

#define PROMPT std::cout << "miniDB << "
#define OUTPUT std::cout << "miniDB >> "

struct InputBuffer {
    std::string buffer;
    size_t buffer_length;
    ssize_t input_length;

    InputBuffer()
        : buffer(""),
          buffer_length(0),
          input_length(0) {}

    InputBuffer(std::string input)
        : buffer(input),
          buffer_length(buffer.length()),
          input_length(buffer.length()) {
    }

    ~InputBuffer() {}

    friend std::ostream& operator<<(std::ostream& out,
                                    const InputBuffer& ib) {
        return out << ib.buffer;
    }

    bool _max_match(const std::string& s) const;

    bool operator==(const std::string& ib) const {
        return _max_match(ib);
    }

    bool operator!=(const std::string& ib) const {
        return _max_match(ib);
    }

    void read_input();
};

#endif