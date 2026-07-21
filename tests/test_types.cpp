#include <iostream>
#include <cassert>
#include <limits>

#include "orderbook/Usings.hpp"
#include "orderbook/Side.hpp"
#include "orderbook/OrderType.hpp"
#include "orderbook/Constants.hpp"
#include "orderbook/TradeInfo.hpp"
#include "orderbook/LevelInfo.hpp"

using namespace orderbook;

int main() {
    std::cout << "[Test] Verifying core types and constants...\n";

    // Test Constants
    assert(Constants::InvalidPrice == std::numeric_limits<Price>::min());

    // Test TradeInfo
    TradeInfo tradeInfo{101, 1500, 50};
    assert(tradeInfo.orderId == 101);
    assert(tradeInfo.price == 1500);
    assert(tradeInfo.quantity == 50);

    // Test LevelInfo
    LevelInfo levelInfo{1500, 200};
    assert(levelInfo.price == 1500);
    assert(levelInfo.quantity == 200);

    // Test Enums
    Side sideBuy = Side::Buy;
    Side sideSell = Side::Sell;
    assert(sideBuy != sideSell);

    OrderType otGtc = OrderType::GoodTillCancel;
    OrderType otMarket = OrderType::Market;
    assert(otGtc != otMarket);

    std::cout << "[Test] All core type assertions passed successfully!\n";
    return 0;
}
