cmake_minimum_required(VERSION 3.14)
project(OrderBookDemo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

find_package(SQLite3 REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)



include_directories(OrderBook)
include_directories(DBHandler)
include_directories(OrderBookWidget)
include_directories(${torch_INCLUDE_DIRS})

message("Going into Orderbook subdirectory...")
add_subdirectory(OrderBook)
message("Going into DBHandler subdirectory...")
add_subdirectory(DBHandler)
message("Going into OrderbookTests subdirectory...")
add_subdirectory(OrderBookTests)
add_subdirectory(OrderBookWidget)

set(SOURCE_FILES main.cpp)
add_executable(OrderBookDemo ${SOURCE_FILES})

target_link_libraries(OrderBookDemo PRIVATE OrderBook DBHandler OrderBookWidget Qt${QT_VERSION_MAJOR}::Widgets SQLite::SQLite3 "${TORCH_LIBRARIES}")
set_property(TARGET OrderBookDemo PROPERTY CXX_STANDARD 17)