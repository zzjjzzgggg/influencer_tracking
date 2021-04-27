cmake_minimum_required(VERSION 3.16)
project(influencers_tracking)


add_library(dyn_dgraph_mgr dyn_dgraph_mgr.cpp)
target_link_libraries(dyn_dgraph_mgr ${LIBRARIES})

add_library(sieve_pag sieve_pag.cpp)
#add_executable(influencers_tracking main.cpp)

#target_link_libraries(sieve_pag dyn_dgraph_mgr)

add_executable(influencers_tracking exam_sieve_pag.cpp)

#add_executable(dyn_dgraph_mgr dyn_dgraph_mgr.cpp)

target_link_libraries(influencers_tracking dyn_dgraph_mgr sieve_pag)

