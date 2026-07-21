#pragma once

#include "LevelInfo.hpp"

namespace orderbook {

class OrderbookLevelInfos {
public:
    OrderbookLevelInfos(const LevelInfos& bids, const LevelInfos& asks)
        : bids_{bids}
        , asks_{asks}
    { }

    [[nodiscard]] const LevelInfos& GetBids() const noexcept { return bids_; }
    [[nodiscard]] const LevelInfos& GetAsks() const noexcept { return asks_; }

private:
    LevelInfos bids_;
    LevelInfos asks_;
};

} // namespace orderbook
