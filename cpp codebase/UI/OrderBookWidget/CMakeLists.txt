project (OrderBookWidget)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${CMAKE_SOURCE_DIR}/Logic/OrderBook")
include_directories("${CMAKE_SOURCE_DIR}/DummyInput/DBHandler")

include(FetchContent)
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.zip
)

FetchContent_MakeAvailable(json)
include_directories(${json_SOURCE_DIR})

add_library(OrderBookWidget STATIC OrderBookWidget.cpp OrderBookWidget.hpp)
target_link_libraries(OrderBookWidget Qt${QT_VERSION_MAJOR}::Widgets nlohmann_json::nlohmann_json)