#pragma once

#include <vector>
#include "Usings.hpp"

namespace orderbook {

struct LevelInfo {
    Price price{0};
    Quantity quantity{0};
};

using LevelInfos = std::vector<LevelInfo>;

} // namespace orderbook
