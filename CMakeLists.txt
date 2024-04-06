cmake_minimum_required(VERSION 3.14)
set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
project(OrderBookDemo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        httplib
        URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.15.0.zip
)

FetchContent_Declare(
        libtorch
        URL https://download.pytorch.org/libtorch/cu118/libtorch-cxx11-abi-shared-with-deps-2.2.1%2Bcu118.zip
)

FetchContent_MakeAvailable(httplib)

FetchContent_GetProperties(libtorch)
if(NOT libtorch_POPULATED)
    FetchContent_Populate(libtorch)
    set(Torch_DIR ${libtorch_SOURCE_DIR}/share/cmake/Torch)
endif()

find_package(Torch REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(${httplib_SOURCE_DIR})
include_directories(${torch_SOURCE_DIR})
include_directories(OrderBook)
include_directories("cpp codebase/Logic/API")
include_directories(DBHandler)
include_directories(OrderBookWidget)

message("Going into Orderbook subdirectory...")
add_subdirectory(OrderBook)
message("Going into DBHandler subdirectory...")
add_subdirectory(DBHandler)
message("Going into OrderbookTests subdirectory...")
#add_subdirectory(OrderBookTests)
add_subdirectory(OrderBookWidget)
message("Going into API subdirectory...")
add_subdirectory("cpp codebase/Logic/API")

set(SOURCE_FILES main.cpp)
add_executable(OrderBookDemo ${SOURCE_FILES})

target_link_libraries(OrderBookDemo PRIVATE OrderBook "cpp codebase/Logic/API" DBHandler OrderBookWidget Qt${QT_VERSION_MAJOR}::Widgets SQLite::SQLite3 httplib::httplib "${TORCH_LIBRARIES}")
set_property(TARGET OrderBookDemo PROPERTY CXX_STANDARD 17)