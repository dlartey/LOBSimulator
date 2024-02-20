project (DBHandler)
message("In DBhandler subdirectory")
add_definitions(-std=c++14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES DBHandler.hpp)
set(SOURCE_FILES DBHandler.cpp)

include_directories("${CMAKE_SOURCE_DIR}/Logic/OrderBook")

message("adding DBHandler library...")
add_library(DBHandler STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(DBHandler PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)