project (OrderBookWidget)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${CMAKE_SOURCE_DIR}/Logic/OrderBook")

add_library(OrderBookWidget STATIC OrderBookWidget.cpp OrderBookWidget.hpp)
target_link_libraries(OrderBookWidget PRIVATE nlohmann_json::nlohmann_json Qt${QT_VERSION_MAJOR}::Widgets)