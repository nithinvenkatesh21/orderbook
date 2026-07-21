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
    assert((TradeInfo{101, 1500, 50}.orderId) == 101);
    assert((TradeInfo{101, 1500, 50}.price) == 1500);
    assert((TradeInfo{101, 1500, 50}.quantity) == 50);

    // Test LevelInfo
    assert((LevelInfo{1500, 200}.price) == 1500);
    assert((LevelInfo{1500, 200}.quantity) == 200);

    // Test Enums
    assert((Side::Buy) != (Side::Sell));
    assert((OrderType::GoodTillCancel) != (OrderType::Market));

    std::cout << "[Test] All core type assertions passed successfully!\n";
    return 0;
}
