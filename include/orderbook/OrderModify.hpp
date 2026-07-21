#pragma once

#include "Order.hpp"

namespace orderbook {

class OrderModify {
public:
    OrderModify(OrderId orderId, Side side, Price price, Quantity quantity)
        : orderId_{orderId}
        , side_{side}
        , price_{price}
        , quantity_{quantity}
    { }

    [[nodiscard]] OrderId GetOrderId() const noexcept { return orderId_; }
    [[nodiscard]] Side GetSide() const noexcept { return side_; }
    [[nodiscard]] Price GetPrice() const noexcept { return price_; }
    [[nodiscard]] Quantity GetQuantity() const noexcept { return quantity_; }

    [[nodiscard]] OrderPointer ToOrderPointer(OrderType type) const {
        return std::make_shared<Order>(type, orderId_, side_, price_, quantity_);
    }

private:
    OrderId orderId_;
    Side side_;
    Price price_;
    Quantity quantity_;
};

} // namespace orderbook
