cmake_minimum_required(VERSION 3.16)
project(influencers_tracking)

set(CMAKE_CXX_STANDARD 17)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{GIT_REPO}/cmake)

find_package(Threads REQUIRED)
find_package(gflags REQUIRED)
find_package(CPPLIB REQUIRED)

add_definitions(${CPPLIB_DEFINITIONS})
include_directories(${CPPLIB_INCLUDE_DIRS})


set(LIBRARIES ${LIBRARIES} gflags ${CPPLIB_LIBRARIES})

message(STATUS "LIBRARIES " ${LIBRARIES})

#add_executable(influencers_tracking  exam_simple_greedy.cpp)
#add_executable(influencers_tracking  exam_sieve_pait.cpp)
add_executable(influencers_tracking  exam_greedy_and_pait.cpp)
#add_executable(influencers_tracking  exam_basic_it.cpp)
#add_executable(influencers_tracking  exam_greedy_and_basicit.cpp)
#add_executable(influencers_tracking exam_histit_seg.cpp)
#add_executable(influencers_tracking exam_hist_and_basic.cpp)
#add_executable(influencers_tracking exam_histit_red.cpp)
#add_executable(influencers_tracking exam_samples.cpp)
#add_executable(influencers_tracking exam_sample_users.cpp)
#add_executable(influencers_tracking exam_lifespan_generator.cpp)
#add_executable(influencers_tracking  exam_random.cpp)
#add_executable(influencers_tracking exam_test.cpp)
#add_executable(influencers_tracking exam_greedy.cpp)

target_link_libraries(influencers_tracking ${LIBRARIES})
