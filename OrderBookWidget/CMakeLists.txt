project (OrderBookWidget)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(OrderBookWidget STATIC OrderBookWidget.cpp OrderBookWidget.hpp)
target_link_libraries(OrderBookWidget Qt${QT_VERSION_MAJOR}::Widgets)