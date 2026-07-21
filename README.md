# High-Performance C++20 Limit Order Book Engine

![CI](https://github.com/nithinvenkatesh21/orderbook/actions/workflows/ci.yml/badge.svg)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

A modern, thread-safe, ultra-low-latency limit order book (LOB) matching engine implemented in standard C++20. Designed for high-frequency trading (HFT) financial systems, supporting order matching algorithms, background thread-safe order pruning, and market depth tracking.

---

## Key Features

- **Order Types Supported:**
  - **Limit Orders (`GoodTillCancel`):** Standard price-time priority limit orders resting in the order book.
  - **Market Orders:** Immediate matching against top-of-book prices across price levels.
  - **Fill-And-Kill (FAK / IOC):** Matches available liquidity immediately and cancels any remaining unfilled quantity.
  - **Fill-Or-Kill (FOK):** Requires full execution against resting depth or cancels entirely with zero fill.
  - **Good-For-Day (GFD):** Automatically pruned at market close via a dedicated RAII background worker thread.
- **Ultra-Low Latency & $O(1)$ Operations:**
  - Fast price-time priority queues via `std::map<Price, OrderPointers>` sorted by price.
  - Constant time $O(1)$ order cancellations via internal iterator lookup table `std::unordered_map<OrderId, OrderEntry>`.
  - Fast level depth aggregation via `std::unordered_map<Price, LevelData>`.
- **Thread Safety:** Thread-safe execution using standard RAII synchronization primitives (`std::scoped_lock`, `std::unique_lock`, `std::condition_variable`).
- **Zero External Dependencies:** Built purely with C++20 standard library and portable CMake build targets.

---

## Performance Benchmarks

Measured on Apple Silicon (M-series / ARM64, single core):

| Metric | Benchmark Result |
| :--- | :--- |
| **Total Orders Processed** | 100,000 orders |
| **Total Trades Executed** | 86,769 trades |
| **Throughput** | **~451,757 operations / sec** |
| **Average Latency per Order** | **~2.21 microseconds (2,213 ns)** |

Run performance benchmarks locally:
```bash
./build/test_benchmark
```

---

## Architecture Overview

```
                        +----------------------------+
                        |      Orderbook Engine      |
                        +----------------------------+
                                      |
         +----------------------------+----------------------------+
         |                                                         |
         v                                                         v
+------------------+                                      +------------------+
|   Bids (Buys)    |  std::map<Price, OrderPointers, >>   |   Asks (Sells)   |  std::map<Price, OrderPointers, <>
+------------------+                                      +------------------+
| $101.00 -> [O4]  |                                      | $103.00 -> [O3]  |
| $100.00 -> [O5]  |                                      | $104.00 -> [O2]  |
| $99.00  -> [O6]  |                                      | $105.00 -> [O1]  |
+------------------+                                      +------------------+
         |                                                         |
         +----------------------------+----------------------------+
                                      |
                                      v
                  +---------------------------------------+
                  | Fast Cancellation & Depth Indexes     |
                  +---------------------------------------+
                  | orders_: OrderId -> Iterator Lookup   |
                  | data_  : Price   -> Aggregate Volume  |
                  +---------------------------------------+
```

---

## Quick Start & Build Instructions

### Prerequisites
- C++20 compliant compiler (GCC 10+, Clang 13+, or MSVC 2019+)
- CMake 3.20+

### Build & Run Tests
```bash
# 1. Clone the repository
git clone https://github.com/nithinvenkatesh21/orderbook.git
cd orderbook

# 2. Configure build with CMake
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release

# 3. Compile executable targets and test suite
cmake --build build --config Release

# 4. Run full unit test suite
ctest --test-dir build --output-on-failure

# 5. Launch demonstration application
./build/orderbook_app
```

---

## Code Example

```cpp
#include <iostream>
#include "orderbook/Orderbook.hpp"

using namespace orderbook;

int main() {
    Orderbook book;

    // Add Limit Sell Order: 50 units at $104
    auto sellOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 1, Side::Sell, 104, 50);
    book.AddOrder(sellOrder);

    // Add Limit Buy Order: 50 units at $104 -> Triggers instant match
    auto buyOrder = std::make_shared<Order>(OrderType::GoodTillCancel, 2, Side::Buy, 104, 50);
    Trades trades = book.AddOrder(buyOrder);

    for (const auto& trade : trades) {
        std::cout << "Executed Trade: " << trade.GetBidTrade().quantity
                  << " units @ $" << trade.GetBidTrade().price << "\n";
    }

    return 0;
}
```

---

## License

Distributed under the MIT License. See `LICENSE.txt` for details.
