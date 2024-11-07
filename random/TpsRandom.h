//
// Created by michal_studia on 12.10.2024.
//

#ifndef PEA_ZAD1_TPSRANDOM_H
#define PEA_ZAD1_TPSRANDOM_H

#include "../AdjMatrix.h"

class TpsRandom{
public:
    static std::vector<int> start_algorithm(AdjMatrix &graph, int exec_time, int optimal_cost);
};


#endif //PEA_ZAD1_TPSRANDOM_H
