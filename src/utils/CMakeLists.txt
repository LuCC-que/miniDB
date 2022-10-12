add_library(utils ./InputBuffer.cpp)

# main also use the header, it has to be public
target_include_directories(utils PUBLIC include)