
#add_test()
project(Tests2)

get_filename_component(PARENT_DIR ../../ ABSOLUTE)

include_directories(${PARENT_DIR}/include )

set(
        SOURCE_FILES
        ${PARENT_DIR}/net/EventLoop.cpp
        ${PARENT_DIR}/net/Channel.cpp
        ${PARENT_DIR}/net/Poller.cpp
        ${PARENT_DIR}/net/KQueuePoller.cpp

        ${PARENT_DIR}/Utility/Logging.cpp
        ${PARENT_DIR}/Utility/CountDownLatch.cpp
        ${PARENT_DIR}/Utility/AsnyLogger.cpp
        ${PARENT_DIR}/Utility/LogFile.cpp
        ${PARENT_DIR}/Utility/FileUtil.cpp
)

add_executable(LoopTest  ${SOURCE_FILES} ./eventLoopTest.cpp )
target_link_libraries(LoopTest PRIVATE fmt::fmt-header-only)
