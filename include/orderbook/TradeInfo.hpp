#pragma once

#include "Usings.hpp"

namespace orderbook {

struct TradeInfo {
    OrderId orderId{0};
    Price price{0};
    Quantity quantity{0};
};

} // namespace orderbook
