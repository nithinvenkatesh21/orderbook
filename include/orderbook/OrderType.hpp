#pragma once

namespace orderbook {

enum class OrderType {
    GoodTillCancel,
    FillAndKill,
    FillOrKill,
    GoodForDay,
    Market
};

} // namespace orderbook
