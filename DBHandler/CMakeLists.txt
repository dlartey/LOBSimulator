project (DBHandler)
message("In DBhandler subdirectory")
add_definitions(-std=c++14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADER_FILES DBHandler.hpp)
set(SOURCE_FILES DBHandler.cpp)

message("adding DBHandler library...")
add_library(DBHandler STATIC ${SOURCE_FILES} ${HEADER_FILES})