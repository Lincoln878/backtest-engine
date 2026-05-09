//
// Created by Shun Hang Lo on 5/7/26.
//

#ifndef BACKTEST_ENGINE_CSVPARSER_H
#define BACKTEST_ENGINE_CSVPARSER_H
#include <vector>
#include <string>
#include <unordered_map>
#include <variant>

namespace csv {
    typedef long long ll;
    typedef std::variant<std::string, double, ll> CsvData;
    typedef std::unordered_map<std::string, std::vector<CsvData>> DataFrame;

    class CsvParser {
    public:
        CsvParser(char c);
        CsvParser(CsvParser const &) = delete;
        CsvParser& operator=(CsvParser const &) = delete;
        CsvParser(CsvParser &&) = default;
        CsvParser& operator=(CsvParser &&) = default;
        ~CsvParser() = default;

        bool parse(std::string filename);
        bool parse(char* filename);
        std::optional<DataFrame> getCsvData(std::string filename);
        template <typename T>
        std::optional<DataFrame> getCsvData(std::string name, T defaultValue);
        std::vector<std::string> getCsvCols();
        size_t getNumRows() const;
        void printData();

    private:
        std::vector<std::string> _columns;
        DataFrame _data;
        size_t _numRows{};
        char _delimiter;
    };
} // csv

#endif //BACKTEST_ENGINE_CSVPARSER_H