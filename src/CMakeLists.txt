add_executable(run
    main.cpp
)


target_link_libraries( run ${SYSTEMC_LIBRARIES})
target_link_libraries( run ${SYSTEMCAMS_LIBRARIES})
