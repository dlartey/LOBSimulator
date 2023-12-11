cmake_minimum_required(VERSION 3.3)
project(OrderBookDemo)

find_package(SQLite3 REQUIRED)
find_package(Qt6 COMPONENTS Widgets REQUIRED)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(SOURCE_FILES main.cpp)
add_executable(OrderBookDemo ${SOURCE_FILES})

include_directories(OrderBook)
include_directories(DBHandler)

message("Going into Orderbook subdirectory...")
add_subdirectory(OrderBook)
message("Going into DBHandler subdirectory...")
add_subdirectory(DBHandler)
message("Going into OrderbookTests subdirectory...")
add_subdirectory(OrderBookTests)

target_link_libraries(OrderBookDemo OrderBook DBHandler SQLite::SQLite3)