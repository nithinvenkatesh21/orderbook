#pragma once

#include <memory>
#include <list>
#include <stdexcept>
#include <string>

#include "OrderType.hpp"
#include "Side.hpp"
#include "Usings.hpp"
#include "Constants.hpp"

namespace orderbook {

class Order {
public:
    Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
        : orderType_{orderType}
        , orderId_{orderId}
        , side_{side}
        , price_{price}
        , initialQuantity_{quantity}
        , remainingQuantity_{quantity}
    { }

    Order(OrderId orderId, Side side, Quantity quantity)
        : Order(OrderType::Market, orderId, side, Constants::InvalidPrice, quantity)
    { }

    [[nodiscard]] OrderId GetOrderId() const noexcept { return orderId_; }
    [[nodiscard]] Side GetSide() const noexcept { return side_; }
    [[nodiscard]] Price GetPrice() const noexcept { return price_; }
    [[nodiscard]] OrderType GetOrderType() const noexcept { return orderType_; }
    [[nodiscard]] Quantity GetInitialQuantity() const noexcept { return initialQuantity_; }
    [[nodiscard]] Quantity GetRemainingQuantity() const noexcept { return remainingQuantity_; }
    [[nodiscard]] Quantity GetFilledQuantity() const noexcept { return initialQuantity_ - remainingQuantity_; }
    [[nodiscard]] bool IsFilled() const noexcept { return remainingQuantity_ == 0; }

    void Fill(Quantity quantity) {
        if (quantity > remainingQuantity_) {
            throw std::logic_error(
                "Order (" + std::to_string(orderId_) +
                ") cannot be filled for quantity (" + std::to_string(quantity) +
                ") exceeding remaining quantity (" + std::to_string(remainingQuantity_) + ")."
            );
        }
        remainingQuantity_ -= quantity;
    }

    void ToGoodTillCancel(Price price) {
        if (orderType_ != OrderType::Market) {
            throw std::logic_error(
                "Order (" + std::to_string(orderId_) +
                ") cannot adjust price unless it is a Market order."
            );
        }
        price_ = price;
        orderType_ = OrderType::GoodTillCancel;
    }

private:
    OrderType orderType_;
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity initialQuantity_;
    Quantity remainingQuantity_;
};

using OrderPointer = std::shared_ptr<Order>;
using OrderPointers = std::list<OrderPointer>;

} // namespace orderbook
