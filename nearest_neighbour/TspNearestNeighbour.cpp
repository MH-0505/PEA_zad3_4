#include <chrono>
#include <vector>
#include <climits>
#include <algorithm>
#include <random>
#include <iostream>
#include "../AdjMatrix.h"
#include "TspNearestNeighbour.h"
// wierzchołek początkowy / losowy, sprawdzić co gdy dwa wierzchołki mają tą samą wartość, rozgałęzienia czyli co kiedy są dwa wierzchołki tej samej wagi (sprawdzić dwie drogi)
// zip ze wszysktim a raport w pdf oddzielnie
std::vector<int> TspNearestNeighbour::start_algorithm(AdjMatrix& graph, int exec_time, int starting_vertex, EqualWeightPickOrder pick_order){
    std::vector<int> results;
    std::vector<int> min_weight_nodes;
    int best_path[graph.vertex_count];
    std::vector<bool> visited_nodes;

    int current_cost = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> range(0, graph.vertex_count - 1);

    auto start = std::chrono::steady_clock::now();

    for(int i = 0; i < graph.vertex_count; i++){
        visited_nodes.push_back(false);
    }
    if(starting_vertex == -1){
        best_path[0] = range(gen);
    } else if(starting_vertex > graph.vertex_count - 1){
        best_path[0] = graph.vertex_count -1;
    } else{
        best_path[0] = starting_vertex;
    }
    visited_nodes[best_path[0]] = true;

    for(int i = 0; i < graph.vertex_count - 1 && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < exec_time; i++){
        int min_weight = INT_MAX;
        min_weight_nodes.clear();
        int next_index = -1;
        for(int j = 0; j < graph.vertex_count; j++){
            if(best_path[i] != j && !visited_nodes[j] && graph.matrix[best_path[i]][j] <= min_weight){
                if(graph.matrix[best_path[i]][j] < min_weight){
                    min_weight_nodes.clear();
                    min_weight = graph.matrix[best_path[i]][j];
                }
                min_weight_nodes.push_back(j);
            }
        }

        if(min_weight_nodes.empty()){
            std::cout << "\nNie znaleziono sciezki. Graf jest rozlaczny 1\n";
            results.push_back(-1);
            return results;
        } else if(min_weight_nodes.size() == 1) {
            next_index = min_weight_nodes.front();
        }else{
            std::uniform_int_distribution<int> temp_range(0, (int)min_weight_nodes.size() - 1);
            switch (pick_order) {
                case FIRST:
                    next_index = min_weight_nodes.front();
                    break;
                case LAST:
                    next_index = min_weight_nodes.back();
                    break;
                case RANDOM:
                    next_index = min_weight_nodes[temp_range(gen)];
                    break;
                case CHECK_BRANCHES:
                    current_cost += min_weight;
                    int branch_min_weight = INT_MAX;
                    std::vector<int> best_branch;
                    for(int index : min_weight_nodes){
                        std::vector<int> branch_result = check_branch(graph, visited_nodes, index, best_path[0], graph.vertex_count - 1 - i, exec_time - std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count());
                        if(branch_result.front() < branch_min_weight){
                            branch_min_weight = branch_result.front();
                            best_branch = branch_result;
                        }
                    }
                    if(best_branch.empty()){
                        std::cout << "\nNie znaleziono sciezki. Graf jest rozlaczny 2\n";
                        results.push_back(-1);
                        return results;
                    }
                    current_cost += best_branch.front();
                    for(int j = 1; j < best_branch.size(); j++){
                        best_path[i+j] = best_branch[j];
                    }
                    results.push_back(current_cost);
                    for(int n : best_path){
                        results.push_back(n);
                    }
                    return results;
            }
        }

        best_path[i+1] = next_index;
        visited_nodes[next_index] = true;
        current_cost += min_weight;
    }
    if(graph.matrix[best_path[graph.vertex_count - 1]][best_path[0]] == -1){
        std::cout << "\nNie znaleziono sciezki. Graf jest  3\n";
        results.push_back(-1);
        return results;
    }

    current_cost += graph.matrix[best_path[graph.vertex_count - 1]][best_path[0]];

    results.push_back(current_cost);
    for(int i = 0; i < graph.vertex_count; i++){
        results.push_back(best_path[i]);
    }

    return results;
}

std::vector<int> TspNearestNeighbour::check_branch(AdjMatrix& graph, std::vector<bool>& old_visited_nodes, int next_vertex, int initial_vertex, int nodes_left, int exec_time){
    std::vector<int> results;
    std::vector<int> min_weight_nodes;
    std::vector<int> best_path;
    std::vector<bool> visited_nodes;
    for(bool b : old_visited_nodes){
        visited_nodes.push_back(b);
    }

    int current_cost = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> range(0, graph.vertex_count - 1);

    auto start = std::chrono::steady_clock::now();

    best_path.push_back(next_vertex);
    visited_nodes[best_path.front()] = true;

    for(int i = 0; i < nodes_left - 1 && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < exec_time; i++){
        int min_weight = INT_MAX;
        min_weight_nodes.clear();
        int next_index = -1;
        for(int j = 0; j < graph.vertex_count; j++){
            if(best_path[i] != j && !visited_nodes[j] && graph.matrix[best_path[i]][j] <= min_weight){
                if(graph.matrix[best_path[i]][j] < min_weight){
                    min_weight_nodes.clear();
                    min_weight = graph.matrix[best_path[i]][j];
                }
                min_weight_nodes.push_back(j);
            }
        }

        if(min_weight_nodes.empty()){
            results.push_back(INT_MAX);
            return results;
        } else if(min_weight_nodes.size() == 1){
            next_index = min_weight_nodes.front();
            best_path.push_back(next_index);
            visited_nodes[next_index] = true;
            current_cost += min_weight;
        } else{
            current_cost += min_weight;
            int branch_min_weight = INT_MAX;
            std::vector<int> best_branch;
            for(int index : min_weight_nodes){
                std::vector<int> branch_result = check_branch(graph, visited_nodes, index, initial_vertex, nodes_left - 1 - i, exec_time - std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count());
                if(branch_result.front() < branch_min_weight){
                    branch_min_weight = branch_result.front();
                    best_branch = branch_result;
                }
            }
            if(best_branch.empty()){
                results.push_back(INT_MAX);
                return results;
            }
            current_cost += best_branch.front();
            for(int j = 1; j < best_branch.size(); j++){
                best_path.push_back(best_branch[j]);
            }
            results.push_back(current_cost);
            for(int n : best_path){
                results.push_back(n);
            }
            return results;
        }
    }
    if(graph.matrix[best_path.back()][initial_vertex] == -1){
        results.push_back(INT_MAX);
        return results;
    }
    current_cost += graph.matrix[best_path.back()][initial_vertex];

    results.push_back(current_cost);
    for(int i : best_path){
        results.push_back(i);
    }
    return results;
}

