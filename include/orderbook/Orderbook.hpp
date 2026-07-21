#pragma once

#include <map>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <optional>

#include "Usings.hpp"
#include "Order.hpp"
#include "OrderModify.hpp"
#include "OrderbookLevelInfos.hpp"
#include "Trade.hpp"

namespace orderbook {

class Orderbook {
private:
    struct OrderEntry {
        OrderPointer order_{nullptr};
        OrderPointers::iterator location_;
    };

    struct LevelData {
        Quantity quantity_{0};
        Quantity count_{0};

        enum class Action {
            Add,
            Remove,
            Match,
        };
    };

    std::unordered_map<Price, LevelData> data_;
    std::map<Price, OrderPointers, std::greater<Price>> bids_;
    std::map<Price, OrderPointers, std::less<Price>> asks_;
    std::unordered_map<OrderId, OrderEntry> orders_;

    mutable std::mutex ordersMutex_;
    std::thread ordersPruneThread_;
    std::condition_variable shutdownConditionVariable_;
    std::atomic<bool> shutdown_{false};

    void PruneGoodForDayOrders();

    void CancelOrders(const OrderIds& orderIds);
    void CancelOrderInternal(OrderId orderId);

    void OnOrderCancelled(OrderPointer order);
    void OnOrderAdded(OrderPointer order);
    void OnOrderMatched(Price price, Quantity quantity, bool isFullyFilled);
    void UpdateLevelData(Price price, Quantity quantity, LevelData::Action action);

    [[nodiscard]] bool CanFullyFill(Side side, Price price, Quantity quantity) const;
    [[nodiscard]] bool CanMatch(Side side, Price price) const;
    Trades MatchOrders();

public:
    Orderbook();
    Orderbook(const Orderbook&) = delete;
    Orderbook& operator=(const Orderbook&) = delete;
    Orderbook(Orderbook&&) = delete;
    Orderbook& operator=(Orderbook&&) = delete;
    ~Orderbook();

    Trades AddOrder(OrderPointer order);
    void CancelOrder(OrderId orderId);
    Trades ModifyOrder(OrderModify order);

    [[nodiscard]] std::size_t Size() const;
    [[nodiscard]] OrderbookLevelInfos GetOrderInfos() const;
};

} // namespace orderbook
