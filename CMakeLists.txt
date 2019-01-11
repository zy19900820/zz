project(zz)
add_definitions("-DLINUX -g -D_GNU_SOURCE")
include_directories(.)

#zz build
aux_source_directory(./zz/ ZZ_DIR_SRCS)
add_library(zz SHARED ${ZZ_DIR_SRCS})

#test build
aux_source_directory(./test/ TEST_DIR_SRCS)
add_executable(test_time ${TEST_DIR_SRCS})
target_link_libraries(test_time gtest_main gtest zz pthread)
