//
// Created by Shun Hang Lo on 5/9/26.
//

#ifndef CSV_PARSER_MARKETDATA_H
#define CSV_PARSER_MARKETDATA_H

namespace mkdt {
    class MarketData {
        MarketData(double open, double close, double high, double low);
        MarketData(const MarketData& other);
        MarketData& operator=(const MarketData& other);
        MarketData(const MarketData&& other) noexcept;
        MarketData& operator=(MarketData&& other) noexcept;

        double open() const;
        double close() const;
        double high() const;
        double low() const;
    private:
        double _open;
        double _close;
        double _high;
        double _low;
    };
} // mkdt

#endif //CSV_PARSER_MARKETDATA_H