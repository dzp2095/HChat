cmake_minimum_required(VERSION 3.15)
project(HChat)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/Utility ${CMAKE_SOURCE_DIR}/net)

set(
        SOURCE_FILES
        Utility/Logging.cpp
        Utility/CountDownLatch.cpp
        Utility/AsnyLogger.cpp
        main.cpp
        Utility/LogFile.cpp
        Utility/FileUtil.cpp
        net/KqueuePoller.cpp
        net/EventLoop.cpp
        net/EpollPoller.cpp
        net/RpcChannel.cpp
        net/Channel.cpp
        net/Poller.cpp)

add_executable(HChat ${SOURCE_FILES} )

target_include_directories(HChat PRIVATE ${YOUR_DIRECTORY})

include_directories("include/")

include_directories(${PROJECT_SOURCE_DIR}/Utility ${PROJECT_SOURCE_DIR}/net)

add_definitions(-std=c++17)

add_subdirectory(Utility/tests EXCLUDE_FROM_ALL)

add_subdirectory(net/tests EXCLUDE_FROM_ALL)

add_subdirectory(include/fmt EXCLUDE_FROM_ALL)

target_link_libraries(HChat PRIVATE fmt::fmt-header-only)

