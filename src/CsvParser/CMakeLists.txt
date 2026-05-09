cmake_minimum_required(VERSION 4.1)
project(csv_parser)

set(CMAKE_CXX_STANDARD 20)
#include_directories(.)

add_executable(
        csv_parser
        main.cpp
        CsvParser.cpp
)
