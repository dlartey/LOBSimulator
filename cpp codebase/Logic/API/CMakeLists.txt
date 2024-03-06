#
# Created by Daniel on 29/01/2024.
#
cmake_minimum_required(VERSION 3.12)
project(API)
set(CMAKE_CXX_STANDARD 11)
include(FetchContent)

FetchContent_Declare(
        httplib
        URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.15.0.zip
)

FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/archive/refs/tags/v3.11.3.zip
)

FetchContent_MakeAvailable(httplib)
FetchContent_MakeAvailable(json)

include_directories(${httplib_SOURCE_DIR})
include_directories(${json_SOURCE_DIR})

set(HEADER_FILES API.hpp)
set(SOURCE_FILES API.cpp)

add_library(API STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(API PRIVATE OrderBook httplib::httplib nlohmann_json::nlohmann_json)