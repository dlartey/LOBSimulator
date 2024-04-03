cmake_minimum_required(VERSION 3.5)
project (OrderBook)
message("In orderbook subdirectory")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(SQLite3 REQUIRED)

set(HEADER_FILES OrderBook.hpp)
set(SOURCE_FILES OrderBook.cpp)

message("adding orderbook library...")

add_library(OrderBook STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(OrderBook Qt${QT_VERSION_MAJOR}::Widgets SQLite::SQLite3)