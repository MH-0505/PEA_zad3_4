#include <chrono>
#include <vector>
#include <climits>
#include <algorithm>
#include <random>
#include <iostream>
#include "../AdjMatrix.h"
#include "TspNearestNeighbour.h"
// wierzchołek początkowy / losowy, sprawdzić co gdy dwa wierzchołki mają tą samą wartość

std::vector<int> TspNearestNeighbour::start_algorithm(AdjMatrix& graph, int exec_time, bool random_start){
    std::vector<int> results;
    int best_path[graph.vertex_count];
    bool visited_nodes[graph.vertex_count];
    int current_cost = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> range(0, graph.vertex_count - 1);

    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < graph.vertex_count; i++){
        visited_nodes[i] = false;
    }
    if(random_start){
        best_path[0] = range(gen);
    } else{
        best_path[0] = 0;
    }
    visited_nodes[best_path[0]] = true;

    for(int i = 0; i < graph.vertex_count - 1 && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < exec_time; i++){
        int min_weight = INT_MAX;
        int min_index = -1;
        for(int j = 0; j < graph.vertex_count; j++){
            if(best_path[i] != j && !visited_nodes[j] && graph.matrix[best_path[i]][j] < min_weight){
                min_weight = graph.matrix[best_path[i]][j];
                min_index = j;
            }
        }
        if(min_index == -1){
            std::cout << "\nNie znaleziono sciezki. Graf jest rozlaczny\n";
            break;
        }
        best_path[i+1] = min_index;
        visited_nodes[min_index] = true;
        current_cost += min_weight;
    }
    current_cost += graph.matrix[best_path[graph.vertex_count - 1]][best_path[0]];

    results.push_back(current_cost);
    for(int i = 0; i < graph.vertex_count; i++){
        results.push_back(best_path[i]);
    }

    return results;
}