#include <iostream>
#include <cassert>
#include <stdexcept>

#include "orderbook/Order.hpp"
#include "orderbook/OrderModify.hpp"
#include "orderbook/Trade.hpp"
#include "orderbook/OrderbookLevelInfos.hpp"

using namespace orderbook;

void TestOrderCreationAndGetters() {
    [[maybe_unused]] Order order(OrderType::GoodTillCancel, 1001, Side::Buy, 150, 10);
    assert(order.GetOrderId() == 1001);
    assert(order.GetSide() == Side::Buy);
    assert(order.GetPrice() == 150);
    assert(order.GetOrderType() == OrderType::GoodTillCancel);
    assert(order.GetInitialQuantity() == 10);
    assert(order.GetRemainingQuantity() == 10);
    assert(order.GetFilledQuantity() == 0);
    assert(!order.IsFilled());
}

void TestOrderFills() {
    Order order(OrderType::GoodTillCancel, 1002, Side::Sell, 200, 50);
    
    order.Fill(20);
    assert(order.GetRemainingQuantity() == 30);
    assert(order.GetFilledQuantity() == 20);
    assert(!order.IsFilled());

    order.Fill(30);
    assert(order.GetRemainingQuantity() == 0);
    assert(order.GetFilledQuantity() == 50);
    assert(order.IsFilled());

    // Fill beyond capacity should throw
    try {
        order.Fill(1);
        assert(false && "Should have thrown std::logic_error");
    } catch (const std::logic_error& ex) {
        std::cout << "  [Expected Exception] " << ex.what() << "\n";
    }
}

void TestMarketOrderConversion() {
    Order marketOrder(1003, Side::Buy, 25);
    assert(marketOrder.GetOrderType() == OrderType::Market);
    assert(marketOrder.GetPrice() == Constants::InvalidPrice);

    marketOrder.ToGoodTillCancel(175);
    assert(marketOrder.GetOrderType() == OrderType::GoodTillCancel);
    assert(marketOrder.GetPrice() == 175);

    // Converting non-market order should throw
    try {
        marketOrder.ToGoodTillCancel(180);
        assert(false && "Should have thrown std::logic_error");
    } catch (const std::logic_error& ex) {
        std::cout << "  [Expected Exception] " << ex.what() << "\n";
    }
}

void TestOrderModifyAndTrade() {
    [[maybe_unused]] OrderModify modify(2001, Side::Sell, 300, 100);
    assert(modify.GetOrderId() == 2001);
    assert(modify.GetSide() == Side::Sell);
    assert(modify.GetPrice() == 300);
    assert(modify.GetQuantity() == 100);

    [[maybe_unused]] auto convertedPtr = modify.ToOrderPointer(OrderType::GoodTillCancel);
    assert(convertedPtr->GetOrderId() == 2001);
    assert(convertedPtr->GetOrderType() == OrderType::GoodTillCancel);

    TradeInfo bidTrade{1, 100, 10};
    TradeInfo askTrade{2, 100, 10};
    [[maybe_unused]] Trade trade(bidTrade, askTrade);
    assert(trade.GetBidTrade().orderId == 1);
    assert(trade.GetAskTrade().orderId == 2);
}

int main() {
    std::cout << "[Test] Running PR 2 domain entity tests...\n";

    TestOrderCreationAndGetters();
    TestOrderFills();
    TestMarketOrderConversion();
    TestOrderModifyAndTrade();

    std::cout << "[Test] All PR 2 domain entity tests passed!\n";
    return 0;
}
