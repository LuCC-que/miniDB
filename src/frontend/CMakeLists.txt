add_library(frontend ./table.cpp ./Cursor.cpp ./SQL_VM.cpp)

target_include_directories(frontend PUBLIC include)