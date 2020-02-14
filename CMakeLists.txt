cmake_minimum_required(VERSION 3.15)
project(.)

set(CMAKE_CXX_STANDARD 14)
include_directories(${CMAKE_SOURCE_DIR}/Utility)

set(
        SOURCE_FILES
        Utility/Logging.cpp
        Utility/CountDownLatch.cpp
        Utility/AsnyLogger.cpp
        main.cpp
)
add_executable(HChat ${SOURCE_FILES} )

target_include_directories(HChat PRIVATE ${YOUR_DIRECTORY})

include_directories("include/")

include_directories(${PROJECT_SOURCE_DIR}/Utility)

add_definitions(-std=c++14)

add_subdirectory(include/fmt EXCLUDE_FROM_ALL)

target_link_libraries(HChat PRIVATE fmt::fmt-header-only)
