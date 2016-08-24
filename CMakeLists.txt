
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

project(Hybrid)

find_package(MPI REQUIRED)
find_package(OpenMP REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

add_executable(hybrid src/hybrid.c)

target_link_libraries(hybrid ${MPI_LIBRARIES})
