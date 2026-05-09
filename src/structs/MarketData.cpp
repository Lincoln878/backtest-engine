//
// Created by Shun Hang Lo on 5/9/26.
//

#include "MarketData.h"

namespace mkdt {
    MarketData::MarketData(double open, double close, double high, double low)
        : _open(open), _close(close), _high(high), _low(low) {}

    MarketData::MarketData(const MarketData &other)
        : _open(other._open), _close(other._close), _high(other._high), _low(other._low) {}

    MarketData::MarketData(const MarketData &&other) noexcept
        : _open(other._open), _close(other._close), _high(other._high), _low(other._low) {}

    double MarketData::open() const { return _open; }
    double MarketData::close() const { return _close; }
    double MarketData::high() const { return _high; }
    double MarketData::low() const { return _low; }
} // mkdt