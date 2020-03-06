
#add_test()
project(Tests)

get_filename_component(PARENT_DIR ../../ ABSOLUTE)

include_directories(${PARENT_DIR}/include )

set(
        SOURCE_FILES
        ${PARENT_DIR}/Utility/Logging.cpp
        ${PARENT_DIR}/Utility/CountDownLatch.cpp
        ${PARENT_DIR}/Utility/AsnyLogger.cpp
        ${PARENT_DIR}/Utility/LogFile.cpp
        ${PARENT_DIR}/Utility/FileUtil.cpp
)

add_executable(ThreadPool  ./ThreadPool.cpp)

add_executable(Logging  ${SOURCE_FILES} ./Logging.cpp)

add_executable(AsynLogger  ${SOURCE_FILES} ./AsynLogger.cpp)

target_link_libraries(Logging PRIVATE fmt::fmt-header-only)
