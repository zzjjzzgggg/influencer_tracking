cmake_minimum_required(VERSION 3.19)
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



#add_library(dyn_dgraph_mgr dyn_dgraph_mgr_v2.cpp lifespan_generator.h exam_lifespan_generator.cpp iset_segment.h hist_pdg.h sieve_pait.h social_network.h subgraph.h)
#target_link_libraries(dyn_dgraph_mgr ${LIBRARIES})


#add_executable(influencers_tracking exam_lifespan_generator.cpp)


#add_executable(influencers_tracking  exam_sieve_pait.cpp)
#add_executable(influencers_tracking exam_greedy.cpp)
add_executable(influencers_tracking  exam_greedy_and_pait.cpp)

#target_link_libraries(influencers_tracking dyn_dgraph_mgr)

#target_link_libraries(influencers_tracking dyn_dgraph_mgr)
