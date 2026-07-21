cmake_minimum_required(VERSION 3.20)
project(Orderbook VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Header-only/interface library for core types (will expand to full library)
add_library(orderbook_core INTERFACE)
target_include_directories(orderbook_core INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Test executables
add_executable(test_types tests/test_types.cpp)
target_link_libraries(test_types PRIVATE orderbook_core)

add_executable(test_orders tests/test_orders.cpp)
target_link_libraries(test_orders PRIVATE orderbook_core)

if(MSVC)
    target_compile_options(test_types PRIVATE /W4 /WX)
    target_compile_options(test_orders PRIVATE /W4 /WX)
else()
    target_compile_options(test_types PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(test_orders PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

enable_testing()
add_test(NAME CoreTypesTest COMMAND test_types)
add_test(NAME DomainOrdersTest COMMAND test_orders)

