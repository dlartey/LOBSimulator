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
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)


include_directories("${CMAKE_SOURCE_DIR}/Logic/OrderBook")
include_directories("${CMAKE_SOURCE_DIR}/DummyInput/DBHandler")
include_directories("${CMAKE_SOURCE_DIR}/UI/OrderBookWidget")
include_directories("${CMAKE_SOURCE_DIR}/UI/CandlestickWidget")
include_directories("${CMAKE_SOURCE_DIR}/UI/HeaderWidget")
include_directories("${CMAKE_SOURCE_DIR}/UI/CentralWidget")
include_directories(${torch_INCLUDE_DIRS})

message("Going into subdirectories...")
add_subdirectory(Logic/OrderBook)
add_subdirectory(DummyInput/DBHandler)
add_subdirectory(Logic/OrderBookTests)

add_subdirectory(UI/OrderBookWidget)
add_subdirectory(UI/CandlestickWidget)
add_subdirectory(UI/HeaderWidget)

add_subdirectory(UI/CentralWidget)
message("added subdirectories")

set(SOURCE_FILES main.cpp)
add_executable(OrderBookDemo ${SOURCE_FILES})

target_link_libraries(OrderBookDemo PRIVATE OrderBook DBHandler OrderBookWidget CandlestickWidget HeaderWidget CentralWidget Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts SQLite::SQLite3 "${TORCH_LIBRARIES}")
set_property(TARGET OrderBookDemo PROPERTY CXX_STANDARD 17)