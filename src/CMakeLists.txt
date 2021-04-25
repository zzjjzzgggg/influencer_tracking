cmake_minimum_required(VERSION 3.16)
project(test_cmake)

set(CMAKE_CXX_STANDARD 14)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{GIT_REPO}/cmake)

find_package(Threads REQUIRED)
find_package(gflags REQUIRED)
find_package(CPPLIB REQUIRED)

add_definitions(${CPPLIB_DEFINITIONS})
include_directories(${CPPLIB_INCLUDE_DIRS})


set(LIBRARIES ${LIBRARIES} gflags ${CPPLIB_LIBRARIES})

message(STATUS "LIBRARIES " ${LIBRARIES})



add_library(dyn_dgraph_mgr dyn_dgraph_mgr.cpp)
target_link_libraries(dyn_dgraph_mgr ${LIBRARIES})


#add_executable(test_cmake main.cpp)

#target_link_libraries(sieve_pag dyn_dgraph_mgr)

add_executable(test_cmake exam_sieve_pag.cpp)

#add_executable(dyn_dgraph_mgr dyn_dgraph_mgr.cpp)

target_link_libraries(test_cmake dyn_dgraph_mgr)

