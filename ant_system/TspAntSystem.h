//
// Created by michal_studia on 18.01.2025.
//

#ifndef PEA_ZAD1_TSPANTSYSTEM_H
#define PEA_ZAD1_TSPANTSYSTEM_H


#include <vector>
#include "../AdjMatrix.h"

class TspAntSystem {
public:
    static std::vector<int> startAlgorithm(AdjMatrix& graph, int exec_time, int num_ants, double alpha, double beta, double evaporation_rate, double pheromone_quantity, int max_iterations, int dorigo_alg);
};


#endif //PEA_ZAD1_TSPANTSYSTEM_H
