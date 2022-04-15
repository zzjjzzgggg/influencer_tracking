add_executable(simple_greedy exam_simple_greedy.cpp)
target_link_libraries(simple_greedy ${LIBRARIES})

add_executable(sieve_pait exam_sieve_pait.cpp)
target_link_libraries(sieve_pait ${LIBRARIES})

add_executable(greedy_and_pait exam_greedy_and_pait.cpp)
target_link_libraries(greedy_and_pait ${LIBRARIES})




set(EXE exam_greedy)
add_executable(${EXE} ${EXE}.cpp)
target_link_libraries(${EXE} ${LIBRARIES})

set(EXE exam_hist)
add_executable(${EXE} exam_hist.cpp)
target_link_libraries(${EXE} ${LIBRARIES})



add_executable(basic_it exam_basic_it.cpp)
target_link_libraries(basic_it ${LIBRARIES})

add_executable(greedy_and_basic exam_greedy_and_basicit.cpp)
target_link_libraries(greedy_and_basic ${LIBRARIES})

add_executable(histit_seg exam_histit_seg.cpp)
target_link_libraries(histit_seg ${LIBRARIES})

add_executable(hist_and_basic exam_hist_and_basic.cpp)
target_link_libraries(hist_and_basic ${LIBRARIES})

add_executable(lifespan_generator exam_lifespan_generator.cpp)
target_link_libraries(lifespan_generator ${LIBRARIES})

add_executable(random exam_random.cpp)
target_link_libraries(random ${LIBRARIES})

add_executable(sample_user exam_sample_users.cpp)
target_link_libraries(sample_user ${LIBRARIES})

add_executable(sample exam_samples.cpp)
target_link_libraries(sample ${LIBRARIES})
