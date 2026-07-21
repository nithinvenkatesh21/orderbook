#include <iostream>
#include <chrono>
#include <vector>
#include <iomanip>
#include <cassert>

#include "orderbook/Orderbook.hpp"

using namespace orderbook;

void RunThroughputBenchmark(size_t numOrders) {
    std::cout << "[Benchmark] Starting Orderbook Performance Benchmark (" << numOrders << " orders)...\n";

    Orderbook book;

    // Pre-create orders to measure matching/insertion performance exclusively
    std::vector<OrderPointer> orders;
    orders.reserve(numOrders);

    for (size_t i = 0; i < numOrders; ++i) {
        OrderId id = i + 1;
        Side side = (i % 2 == 0) ? Side::Buy : Side::Sell;
        Price price = 100 + static_cast<Price>(i % 20); // Price levels between 100 and 119
        Quantity quantity = 10 + static_cast<Quantity>(i % 50);

        orders.push_back(std::make_shared<Order>(OrderType::GoodTillCancel, id, side, price, quantity));
    }

    size_t totalTradesCount = 0;

    auto startTime = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < numOrders; ++i) {
        auto trades = book.AddOrder(orders[i]);
        totalTradesCount += trades.size();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationMs = endTime - startTime;

    double seconds = durationMs.count() / 1000.0;
    double opsPerSec = static_cast<double>(numOrders) / seconds;
    double avgLatencyNs = (durationMs.count() * 1000000.0) / static_cast<double>(numOrders);

    std::cout << "====================================================\n";
    std::cout << "               BENCHMARK RESULTS\n";
    std::cout << "====================================================\n";
    std::cout << "  Total Orders Processed: " << numOrders << "\n";
    std::cout << "  Total Trades Executed:  " << totalTradesCount << "\n";
    std::cout << "  Elapsed Time:           " << std::fixed << std::setprecision(3) << durationMs.count() << " ms\n";
    std::cout << "  Throughput:             " << std::fixed << std::setprecision(0) << opsPerSec << " ops/sec\n";
    std::cout << "  Avg Latency per Order:  " << std::fixed << std::setprecision(2) << avgLatencyNs << " ns\n";
    std::cout << "====================================================\n";

    assert(opsPerSec > 10000.0 && "Throughput should exceed 10,000 ops/sec");
}

int main() {
    RunThroughputBenchmark(100000);
    return 0;
}
