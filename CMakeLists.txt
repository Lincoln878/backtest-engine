cmake_minimum_required(VERSION 4.1)
project(backtest_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(backtest_engine
        src/main.cpp
        src/CsvParser.cpp
        src/CsvParser.h)
