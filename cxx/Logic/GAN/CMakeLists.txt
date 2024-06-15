#
# Created by Daniel on 29/01/2024.
#
cmake_minimum_required(VERSION 3.18)
project(GAN)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES GAN.hpp)
set(SOURCE_FILES GAN.cpp)

add_library(GAN STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(GAN PRIVATE "${TORCH_LIBRARIES}" OrderBook API)
set_property(TARGET GAN PROPERTY CXX_STANDARD 17)