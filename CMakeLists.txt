cmake_minimum_required(VERSION 3.5.1)
project(mi_pdp_cpp)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES main.cpp Graph.cpp Graph.h Bipartite.cpp Bipartite.h Color.h Solver.cpp Solver.h)
add_executable(mi_pdp_cpp ${SOURCE_FILES})