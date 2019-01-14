project(zz)
add_definitions("-DLINUX -g -D_GNU_SOURCE")
include_directories(.)

#zz build
aux_source_directory(./src/ ZZ_DIR_SRCS)
add_library(zz SHARED ${ZZ_DIR_SRCS})

function(test name src)
  FILE(GLOB TEST_SRC ${src})
  add_executable(${name} ${TEST_SRC})
  target_link_libraries(${name} gtest_main gtest zz pthread)
endfunction()

test(test_time ./test/test_time.cpp)
test(test_log ./test/test_log.cpp)
test(test_baseclss ./test/test_baseclass.cpp)
test(test_file ./test/test_file.cpp)
test(test_config_read ./test/test_config_read.cpp)
test(test_thread ./test/test_thread.cpp)
test(test_signaler ./test/test_signaler.cpp)
