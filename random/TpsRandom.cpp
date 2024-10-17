#include <chrono>
#include <vector>
#include <climits>
#include <algorithm>
#include <random>
#include "TpsRandom.h"
#include "../AdjMatrix.h"

std::vector<int> TpsRandom::start_algorithm(AdjMatrix& graph, int exec_time){
    std::vector<int> results;
    int best_path[graph.vertex_count];
    int current_path[graph.vertex_count];
    int lowest_cost = INT_MAX;
    int current_cost = 0;

    for(int i = 0; i < graph.vertex_count; i++){
        current_path[i] = i;
    }


    auto start = std::chrono::steady_clock::now();
    std::shuffle(current_path, current_path+graph.vertex_count, std::mt19937{std::random_device{}()});

    do{
        current_cost = graph.matrix[current_path[graph.vertex_count - 1]][current_path[0]];
        for(int i = 1; i < graph.vertex_count; i++){
            current_cost += graph.matrix[current_path[i-1]][current_path[i]];
        }
        if(current_cost < lowest_cost){
            lowest_cost = current_cost;
            for(int i = 0; i < graph.vertex_count; i++){
                best_path[i] = current_path[i];
            }
        }
        std::shuffle(current_path+1, current_path+graph.vertex_count, std::mt19937{std::random_device{}()});

    }while(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < exec_time);

    results.push_back(lowest_cost);
    for(int i = 0; i < graph.vertex_count; i++){
        results.push_back(best_path[i]);
    }

    return results;
}