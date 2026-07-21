#include <iostream>
#include <cassert>

#include "orderbook/Orderbook.hpp"

using namespace orderbook;

void TestSimpleLimitMatch() {
    std::cout << "  - Running TestSimpleLimitMatch...\n";
    Orderbook book;

    // Add Sell order: Price 100, Quantity 10
    auto sellOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Sell, 100, 10);
    auto trades1 = book.AddOrder(sellOrder);
    assert(trades1.empty());
    assert(book.Size() == 1);

    // Add Buy order: Price 100, Quantity 10
    auto buyOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 2, Side::Buy, 100, 10);
    auto trades2 = book.AddOrder(buyOrder);
    assert(trades2.size() == 1);
    assert(book.Size() == 0);

    const auto& trade = trades2.front();
    assert(trade.GetBidTrade().orderId == 2);
    assert(trade.GetAskTrade().orderId == 1);
    assert(trade.GetBidTrade().quantity == 10);
    assert(trade.GetBidTrade().price == 100);
}

void TestPartialFillAndCancel() {
    std::cout << "  - Running TestPartialFillAndCancel...\n";
    Orderbook book;

    // Add Sell order: Price 200, Quantity 50
    auto sellOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 10, Side::Sell, 200, 50);
    book.AddOrder(sellOrder);

    // Add Buy order: Price 200, Quantity 20 (partial fill)
    auto buyOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 11, Side::Buy, 200, 20);
    auto trades = book.AddOrder(buyOrder);
    assert(trades.size() == 1);
    assert(sellOrder->GetRemainingQuantity() == 30);
    assert(book.Size() == 1);

    // Cancel remaining sell order
    book.CancelOrder(10);
    assert(book.Size() == 0);
}

void TestOrderModify() {
    std::cout << "  - Running TestOrderModify...\n";
    Orderbook book;

    auto buyOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 100, Side::Buy, 50, 100);
    book.AddOrder(buyOrder);
    assert(book.Size() == 1);

    // Modify order 100: new price 55, new quantity 150
    OrderModify modifyReq(100, Side::Buy, 55, 150);
    book.ModifyOrder(modifyReq);
    assert(book.Size() == 1);

    auto infos = book.GetOrderInfos();
    assert(infos.GetBids().size() == 1);
    assert(infos.GetBids().front().price == 55);
    assert(infos.GetBids().front().quantity == 150);
}

int main() {
    std::cout << "[Test] Running PR 3 Orderbook Matching Engine tests...\n";

    TestSimpleLimitMatch();
    TestPartialFillAndCancel();
    TestOrderModify();

    std::cout << "[Test] All PR 3 Orderbook Matching Engine tests passed!\n";
    return 0;
}
