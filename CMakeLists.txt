cmake_minimum_required(VERSION 3.20)
project(Orderbook VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find Threads library for POSIX/Linux pthreads
find_package(Threads REQUIRED)

# Orderbook Core Library
add_library(orderbook STATIC src/Orderbook.cpp)
target_include_directories(orderbook PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_link_libraries(orderbook PUBLIC Threads::Threads)

# Driver Application Executable
add_executable(orderbook_app src/main.cpp)
target_link_libraries(orderbook_app PRIVATE orderbook)

# Test Executables
add_executable(test_types tests/test_types.cpp)
target_link_libraries(test_types PRIVATE orderbook)

add_executable(test_orders tests/test_orders.cpp)
target_link_libraries(test_orders PRIVATE orderbook)

add_executable(test_orderbook tests/test_orderbook.cpp)
target_link_libraries(test_orderbook PRIVATE orderbook)

add_executable(test_benchmark tests/test_benchmark.cpp)
target_link_libraries(test_benchmark PRIVATE orderbook)

if(MSVC)
    target_compile_options(orderbook PRIVATE /W4 /WX)
    target_compile_options(orderbook_app PRIVATE /W4 /WX)
    target_compile_options(test_types PRIVATE /W4 /WX)
    target_compile_options(test_orders PRIVATE /W4 /WX)
    target_compile_options(test_orderbook PRIVATE /W4 /WX)
    target_compile_options(test_benchmark PRIVATE /W4 /WX)
else()
    target_compile_options(orderbook PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(orderbook_app PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(test_types PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(test_orders PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(test_orderbook PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(test_benchmark PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

enable_testing()
add_test(NAME CoreTypesTest COMMAND test_types)
add_test(NAME DomainOrdersTest COMMAND test_orders)
add_test(NAME MatchingEngineTest COMMAND test_orderbook)
add_test(NAME BenchmarkTest COMMAND test_benchmark)
