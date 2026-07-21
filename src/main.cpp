#include <iostream>
#include <iomanip>

#include "orderbook/Orderbook.hpp"

using namespace orderbook;

void PrintOrderbookState(const Orderbook& book) {
    auto levelInfos = book.GetOrderInfos();
    std::cout << "\n================ ORDERBOOK SNAPSHOT ================\n";
    std::cout << "  ASKS (Sells):\n";
    const auto& asks = levelInfos.GetAsks();
    if (asks.empty()) {
        std::cout << "    [Empty]\n";
    } else {
        for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
            std::cout << "    Price: $" << std::setw(6) << it->price
                      << " | Quantity: " << std::setw(6) << it->quantity << "\n";
        }
    }

    std::cout << "  --------------------------------------------------\n";
    std::cout << "  BIDS (Buys):\n";
    const auto& bids = levelInfos.GetBids();
    if (bids.empty()) {
        std::cout << "    [Empty]\n";
    } else {
        for (const auto& level : bids) {
            std::cout << "    Price: $" << std::setw(6) << level.price
                      << " | Quantity: " << std::setw(6) << level.quantity << "\n";
        }
    }
    std::cout << "====================================================\n\n";
}

int main() {
    std::cout << "====================================================\n";
    std::cout << "      High-Performance C++20 Orderbook Engine\n";
    std::cout << "====================================================\n";

    Orderbook book;

    std::cout << "\n[1] Submitting Initial Limit Orders...\n";
    book.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Sell, 105, 100));
    book.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 2, Side::Sell, 104, 50));
    book.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 3, Side::Sell, 103, 75));

    book.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 4, Side::Buy, 101, 80));
    book.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 5, Side::Buy, 100, 150));
    book.AddOrder(std::make_shared<Order>(OrderType::GoodTillCancel, 6, Side::Buy, 99, 200));

    PrintOrderbookState(book);

    std::cout << "[2] Submitting Aggressive Buy Order at $104 for 100 units...\n";
    auto buyOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 7, Side::Buy, 104, 100);
    auto trades = book.AddOrder(buyOrder);

    std::cout << "    Trades Executed: " << trades.size() << "\n";
    for (size_t i = 0; i < trades.size(); ++i) {
        const auto& t = trades[i];
        std::cout << "      Trade #" << (i + 1)
                  << ": Bid Order (" << t.GetBidTrade().orderId << ") vs Ask Order (" << t.GetAskTrade().orderId << ")"
                  << " | Price: $" << t.GetBidTrade().price
                  << " | Qty: " << t.GetBidTrade().quantity << "\n";
    }

    PrintOrderbookState(book);

    std::cout << "[3] Submitting Market Order to Sell 50 units...\n";
    auto marketSell = std::make_shared<Order>(8, Side::Sell, 50);
    auto marketTrades = book.AddOrder(marketSell);
    std::cout << "    Trades Executed: " << marketTrades.size() << "\n";
    for (size_t i = 0; i < marketTrades.size(); ++i) {
        const auto& t = marketTrades[i];
        std::cout << "      Trade #" << (i + 1)
                  << ": Bid Order (" << t.GetBidTrade().orderId << ") vs Ask Order (" << t.GetAskTrade().orderId << ")"
                  << " | Price: $" << t.GetBidTrade().price
                  << " | Qty: " << t.GetBidTrade().quantity << "\n";
    }

    PrintOrderbookState(book);

    std::cout << "Orderbook application demonstration completed successfully.\n";
    return 0;
}
