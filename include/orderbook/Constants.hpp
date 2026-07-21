#pragma once

#include <limits>
#include "Usings.hpp"

namespace orderbook {

namespace Constants {
    constexpr Price InvalidPrice = std::numeric_limits<Price>::min();
}

} // namespace orderbook
