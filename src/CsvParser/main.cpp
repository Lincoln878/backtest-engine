//
// Created by Shun Hang Lo on 5/9/26.
//
#include "CsvParser.h"
#include <iostream>

int main(const int argc, char** argv) {
    if (argc != 2) return -1;
    std::string filepath = argv[1];
    std::cout << "Parsing file: " << filepath << std::endl;

    csv::CsvParser csv(',');
    csv.parse(filepath);

    csv.printData();
    std::cout << "rows: " << csv.getNumRows() << '\n';
}
