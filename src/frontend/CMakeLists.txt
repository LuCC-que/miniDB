add_library(frontend ./table.cpp ./SQL_VM.cpp ./Cursor.cpp)

target_include_directories(frontend PUBLIC include)