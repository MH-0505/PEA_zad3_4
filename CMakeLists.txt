cmake_minimum_required(VERSION 3.28)
project(PEA_zad1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(brute_force brute_force/main_bf.cpp
        AdjMatrix.cpp
        brute_force/TpsBruteForce.cpp
        brute_force/TpsBruteForce.h
        ConfigManager.cpp
        ConfigManager.h)
add_executable(random random/main_r.cpp
        AdjMatrix.cpp
        random/TpsRandom.cpp
        random/TpsRandom.h
        ConfigManager.cpp
        ConfigManager.h)
add_executable(nearest_neighbour nearest_neighbour/main_nn.cpp
        AdjMatrix.cpp
        ConfigManager.cpp
        ConfigManager.h
        nearest_neighbour/TspNearestNeighbour.cpp
        nearest_neighbour/TspNearestNeighbour.h)
