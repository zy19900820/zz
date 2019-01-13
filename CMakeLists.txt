project(zz)
add_definitions("-DLINUX -g -D_GNU_SOURCE")
include_directories(.)

#zz build
aux_source_directory(./src/ ZZ_DIR_SRCS)
add_library(zz SHARED ${ZZ_DIR_SRCS})

#test build
FILE(GLOB TEST_TIME_SRC ./test/test_time.cpp)
add_executable(test_time ${TEST_TIME_SRC})
target_link_libraries(test_time gtest_main gtest zz pthread)

FILE(GLOB TEST_LOG_SRC ./test/test_log.cpp)
add_executable(test_log ${TEST_LOG_SRC})
target_link_libraries(test_log gtest_main gtest zz pthread)
