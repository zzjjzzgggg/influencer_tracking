cmake_minimum_required(VERSION 3.16)
project(influencer_tracking)

set(CMAKE_CXX_STANDARD 14)
#set(PATH "/home/weiwei/code/GIT_REPO")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{GIT_REPO}/cmake)
message(STATUS "CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

find_package(Threads REQUIRED)
find_package(gflags REQUIRED)
find_package(CPPLIB REQUIRED)
#set(CPPLIB_DIR /home/weiwei/code/GIT_REPO/cpplib)
add_definitions(${CPPLIB_DEFINITIONS})
include_directories(${CPPLIB_INCLUDE_DIRS})

#include_directories(${CPPLIB_DIR})
#link_libraries(cpplib)
add_executable(influencer_tracking main.cpp stdafx.h bernoulli_get.h social_stream.h social_action.h  sieve_pag.h sieve_pag.cpp input_mgr.h dyn_dgraph_mgr.h dyn_dgraph_mgr.cpp  candidate.h)

#[[set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})]]

#target_link_libraries(influencer_tracking CPPLIB_DIR)




