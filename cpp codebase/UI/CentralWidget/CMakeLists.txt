project (CentralWidget)
add_definitions(-std=c++14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${CMAKE_SOURCE_DIR}/UI/CandlestickWidget")
include_directories("${CMAKE_SOURCE_DIR}/UI/OrderBookWidget")

add_library(CentralWidget STATIC CentralWidget.cpp CentralWidget.hpp)
target_link_libraries(CentralWidget Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Charts)