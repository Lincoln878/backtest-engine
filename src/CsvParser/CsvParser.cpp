//
// Created by Shun Hang Lo on 5/7/26.
//

#include "CsvParser.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>

namespace csv {
    CsvParser::CsvParser(const char c)
        : _delimiter(c) {}

    size_t CsvParser::getNumRows() const {
        return _numRows;
    }

    static std::vector<CsvData> _split(const std::string& line, const char delimiter) {
        if (line.empty()) return {};
        std::vector<CsvData> res;
        std::string tmp;
        int digit_c = 0, dot_c = 0;
        for (const auto& c : line) {
            if (c == delimiter) {
                if (digit_c == static_cast<int>(tmp.length()) && !tmp.empty())
                    res.emplace_back(std::stoll(tmp));
                else if (
                        dot_c == 1 &&
                        digit_c == static_cast<int>(tmp.length()) - 1 &&
                        !tmp.empty() &&
                        tmp.front() != '.' &&
                        tmp.back() != '.'
                    )
                    res.emplace_back(std::stod(tmp));
                else
                    res.emplace_back(tmp);

                tmp.clear();
                digit_c = dot_c = 0;
            } else {
                if (c >= '0' && c <= '9') digit_c++;
                if (c == '.') dot_c++;
                tmp += c;
            }
        }
        if (!tmp.empty()) {
            if (digit_c == static_cast<int>(tmp.length()) && !tmp.empty())
                res.emplace_back(std::stoll(tmp));
            else if (dot_c == 1 && digit_c == static_cast<int>(tmp.length()) - 1 && tmp.front() != '.' && tmp.back() != '.')
                res.emplace_back(std::stod(tmp));
            else
                res.emplace_back(tmp);
        }

        return res;
    }

    static std::vector<std::string> split(const std::string& line, const char delimiter) {
        if (line.empty()) return {};
        std::vector<std::string> res;
        std::string tmp;
        for (const auto& c : line) {
            if (c == delimiter) {
                res.emplace_back(tmp);
                tmp.clear();
            } else {
                tmp += c;
            }
        }
        if (!tmp.empty()) res.emplace_back(tmp);

        return res;
    }

    // Helper: convert CsvData -> string with fixed precision for doubles
    static std::string csvDataToString(const CsvData& v, int floatPrecision = 6) {
        return std::visit([&](auto const &val) -> std::string {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return val;
            } else if constexpr (std::is_same_v<T, double>) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(floatPrecision) << val;
                return oss.str();
            } else {
                // assume integral (ll)
                return std::to_string(val);
            }
        }, v);
    }

    bool CsvParser::parse(std::string filename) {
        std::string line;
        std::ifstream file(filename, std::ios::in);
        _numRows = 0;

        if (!file.is_open()) return false;

        // Get the columns of the CSV (first line)
        if (!std::getline(file, line)) return false;
        _columns = split(line, _delimiter);

        // Parse contents of the csv
        while (std::getline(file, line)) {
            std::vector<CsvData> tmp = _split(line, _delimiter);
            for (size_t i = 0; i < _columns.size(); i++) {
                _data[_columns[i]].emplace_back(std::move(tmp[i]));
            }
            _numRows++;
        }


        return true;
    }

    bool CsvParser::parse(char* filename) {
        return parse(std::string(filename));
    }

    void CsvParser::printData() {
        const auto& cols = _columns;
        size_t ncol = cols.size();

        // compute widths: start from header widths
        std::vector<size_t> widths(ncol);
        for (size_t i = 0; i < ncol; ++i) widths[i] = cols[i].size();

        // determine number of rows (max column length)
        size_t rows = 0;
        for (auto const &col : cols) {
            auto it = _data.find(col);
            if (it != _data.end()) rows = std::max(rows, it->second.size());
        }

        const int floatPrecision = 6;

        // compute max width per column
        for (size_t c = 0; c < ncol; ++c) {
            auto it = _data.find(cols[c]);
            if (it == _data.end()) continue;
            for (const auto &cell : it->second) {
                std::string s = csvDataToString(cell, floatPrecision);
                widths[c] = std::max(widths[c], s.size());
            }
        }

        auto& os = std::cout;

        // print header
        for (size_t c = 0; c < ncol; ++c) {
            if (c) os << " | ";
            os << std::left << std::setw(static_cast<int>(widths[c])) << cols[c];
        }
        os << '\n';

        // separator
        for (size_t c = 0; c < ncol; ++c) {
            if (c) os << "-+-";
            os << std::string(widths[c], '-');
        }
        os << '\n';

        // rows
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = 0; c < ncol; ++c) {
                if (c) os << " | ";
                auto it = _data.find(cols[c]);
                if (it == _data.end() || r >= it->second.size()) {
                    os << std::left << std::setw(static_cast<int>(widths[c])) << "";
                    continue;
                }
                const CsvData &cell = it->second[r];
                // choose alignment by contained type
                if (std::holds_alternative<std::string>(cell)) {
                    os << std::left << std::setw(static_cast<int>(widths[c])) << csvDataToString(cell, floatPrecision);
                } else {
                    os << std::right << std::setw(static_cast<int>(widths[c])) << csvDataToString(cell, floatPrecision);
                }
            }
            os << '\n';
        }
    }

} // csv