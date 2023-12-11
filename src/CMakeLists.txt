add_subdirectory(perf)
add_subdirectory(algebra)


add_executable(main main.cpp)
target_link_libraries(main perf algebra)
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)



add_executable(gemm gemm.cpp)
target_link_libraries(gemm algebra)
set_target_properties(gemm PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
