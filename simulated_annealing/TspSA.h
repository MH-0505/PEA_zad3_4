//
// Created by micha on 18.12.2024.
//

#ifndef PEA_ZAD1_TSPSA_H
#define PEA_ZAD1_TSPSA_H


#include "../AdjMatrix.h"

class TspSA {
public:
    static std::vector<int> startAlgorithm(AdjMatrix& graph, double initial_temp, double minimum_temperature, double cooling_rate, int exec_time, int max_iterations,int epoch, int annealing_mode, int solution_gen_mode, bool random_start_path);

private:
    static int calculateRouteCost(AdjMatrix &graph, const std::vector<int> &route);
};


#endif //PEA_ZAD1_TSPSA_H
