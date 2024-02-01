cmake_minimum_required(VERSION 3.5)
project(OrderBookDemo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        httplib
        URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.15.0.zip
)

FetchContent_MakeAvailable(httplib)

find_package(SQLite3 REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${httplib_SOURCE_DIR})
include_directories(OrderBook)
include_directories(API)
include_directories(DBHandler)
include_directories(OrderBookWidget)

message("Going into Orderbook subdirectory...")
add_subdirectory(OrderBook)
message("Going into DBHandler subdirectory...")
add_subdirectory(DBHandler)
message("Going into OrderbookTests subdirectory...")
#add_subdirectory(OrderBookTests)
add_subdirectory(OrderBookWidget)
message("Going into API subdirectory...")
add_subdirectory(API)

set(SOURCE_FILES main.cpp)
add_executable(OrderBookDemo ${SOURCE_FILES})

target_link_libraries(OrderBookDemo PRIVATE OrderBook API DBHandler OrderBookWidget Qt${QT_VERSION_MAJOR}::Widgets SQLite::SQLite3 httplib::httplib)
