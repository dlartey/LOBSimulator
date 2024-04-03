#
# Created by Daniel on 29/01/2024.
#
cmake_minimum_required(VERSION 3.12)
project(API)
set(CMAKE_CXX_STANDARD 11)

set(HEADER_FILES API.hpp)
set(SOURCE_FILES API.cpp)

add_library(API STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(API PRIVATE OrderBook nlohmann_json::nlohmann_json httplib::httplib)