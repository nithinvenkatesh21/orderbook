#pragma once

#include <vector>
#include "TradeInfo.hpp"

namespace orderbook {

class Trade {
public:
    Trade(const TradeInfo& bidTrade, const TradeInfo& askTrade)
        : bidTrade_{bidTrade}
        , askTrade_{askTrade}
    { }

    [[nodiscard]] const TradeInfo& GetBidTrade() const noexcept { return bidTrade_; }
    [[nodiscard]] const TradeInfo& GetAskTrade() const noexcept { return askTrade_; }

private:
    TradeInfo bidTrade_;
    TradeInfo askTrade_;
};

using Trades = std::vector<Trade>;

} // namespace orderbook
