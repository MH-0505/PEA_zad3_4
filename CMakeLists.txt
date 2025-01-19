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
        brute_force/TpsBruteForce.cpp
        brute_force/TpsBruteForce.h
        ConfigManager.cpp
        ConfigManager.h)
add_executable(nearest_neighbour nearest_neighbour/main_nn.cpp
        AdjMatrix.cpp
        ConfigManager.cpp
        ConfigManager.h
        brute_force/TpsBruteForce.cpp
        brute_force/TpsBruteForce.h
        nearest_neighbour/TspNearestNeighbour.cpp
        nearest_neighbour/TspNearestNeighbour.h)
add_executable(branch_and_bound branch_and_bound/main_bb.cpp
        AdjMatrix.cpp
        ConfigManager.cpp
        ConfigManager.h
        brute_force/TpsBruteForce.cpp
        brute_force/TpsBruteForce.h
        branch_and_bound/main_bb.cpp
        branch_and_bound/TspBranchAndBound.h
        branch_and_bound/TspBranchAndBound.cpp)
add_executable(simulated_annealing simulated_annealing/main_sa.cpp
        simulated_annealing/TspSA.cpp
        simulated_annealing/TspSA.h
        AdjMatrix.cpp
        ConfigManager.cpp
        ConfigManager.h
        brute_force/TpsBruteForce.cpp
        brute_force/TpsBruteForce.h
        nearest_neighbour/TspNearestNeighbour.cpp
        nearest_neighbour/TspNearestNeighbour.h)
add_executable(ant_system ant_system/main_as.cpp
        ant_system/TspAntSystem.cpp
        ant_system/TspAntSystem.h
        AdjMatrix.cpp
        ConfigManager.cpp
        ConfigManager.h
        brute_force/TpsBruteForce.cpp
        brute_force/TpsBruteForce.h)
