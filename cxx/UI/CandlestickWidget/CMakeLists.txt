project (CandlestickWidget)
add_definitions(-std=c++14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${CMAKE_SOURCE_DIR}/Logic/OrderBook")

add_library(CandlestickWidget STATIC CandlestickWidget.cpp CandlestickWidget.hpp)
target_link_libraries(CandlestickWidget Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)