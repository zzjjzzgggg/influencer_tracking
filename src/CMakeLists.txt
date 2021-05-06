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



add_library(dyn_dgraph_mgr dyn_dgraph_mgr_v2.cpp lifespan_generator.h exam_lifespan_generator.cpp bernoulli_segment.h)
target_link_libraries(dyn_dgraph_mgr ${LIBRARIES})

add_library(sieve_pag sieve_pag.cpp)
#add_executable(influencers_tracking main.cpp)

#target_link_libraries(sieve_pag dyn_dgraph_mgr)

#add_executable(influencers_tracking exam_sieve_and_greedy.cpp)
#add_executable(influencers_tracking exam_greedy_pag.cpp)
#add_executable(influencers_tracking exam_lifespan_generator.cpp)
add_executable(influencers_tracking exam_basic_pdg.cpp)

#add_executable(dyn_dgraph_mgr dyn_dgraph_mgr.cpp)

target_link_libraries(influencers_tracking dyn_dgraph_mgr sieve_pag)

#target_link_libraries(influencers_tracking dyn_dgraph_mgr)
