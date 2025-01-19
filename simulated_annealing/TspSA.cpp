//
// Created by micha on 18.12.2024.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <random>
#include <chrono>
#include "TspSA.h"
#include "../AdjMatrix.h"
#include "../nearest_neighbour/TspNearestNeighbour.h"


int TspSA::calculateRouteCost(AdjMatrix& graph, const std::vector<int>& route) {
    int cost = 0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        cost += graph.matrix[route[i]][route[i + 1]];
    }

    cost += graph.matrix[route.back()][route.front()];
    return cost;
}

std::vector<int> TspSA::startAlgorithm(AdjMatrix& graph, double initial_temp, double minimum_temperature, double cooling_rate, int exec_time, long long int max_iterations, int epoch, int annealing_mode, int solution_generator_mode, bool random_start_path) {
    std::vector<int> bestSolution = TspNearestNeighbour::start_algorithm(graph, 900, -1, TspNearestNeighbour::RANDOM);
    //std::vector<int> bestSolution = TspNearestNeighbour::start_algorithm(graph, 900, 4, TspNearestNeighbour::FIRST);
    long long int maxIterations;

    if(max_iterations == 0){
        maxIterations = LLONG_MAX;
    } else{
        maxIterations = max_iterations;
    }

    int bestCost = bestSolution[0];
    int currentCost = bestSolution[0];
    std::vector<int> currentSolution(graph.vertex_count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> range(0, graph.vertex_count - 1);
    std::uniform_real_distribution<double> randomDouble(0.0, 1.0);

    if(random_start_path){
        for(int i = 0; i < graph.vertex_count; i++){
            currentSolution[i] = i;
        }
        std::shuffle(currentSolution.begin(), currentSolution.end(), gen);
        currentCost = calculateRouteCost(graph, currentSolution);
    }else {
        for(int i = 0; i < bestSolution.size()-1; i++){
            currentSolution[i] = bestSolution[i+1];
        }
    }


    double temperature = initial_temp;
    long long int iteration = 0;

    auto start = std::chrono::steady_clock::now();

    do {
        iteration++;
        std::vector<int> newSolution = currentSolution;
        if(solution_generator_mode == 0){
            int i = range(gen);
            int j = range(gen);
            std::swap(newSolution[i], newSolution[j]);
        }else if(solution_generator_mode == 1){
            int i = range(gen);
            int j = range(gen);
            if (i > j) std::swap(i, j);
            std::shuffle(newSolution.begin() + i, newSolution.begin() + j + 1, gen);
        }


        int newCost = calculateRouteCost(graph, newSolution);
        int deltaCost = newCost - currentCost;

        if (deltaCost < 0 || randomDouble(gen) < std::exp(-deltaCost / temperature)) {  // kryterium akceptacji
            currentSolution = newSolution;
            currentCost = newCost;

            if (currentCost < bestCost) {
                bestSolution[0] = currentCost;
                int n = 1;
                for(int vertex : currentSolution){
                    bestSolution[n] = vertex;
                    n++;
                }
            }
        }

        if(iteration % epoch == 0){
            if(annealing_mode == 0){
                temperature *= cooling_rate;
            } else if(annealing_mode == 1){
                temperature = initial_temp / (1 + (1-cooling_rate) * std::log(iteration));
            }
        }
    } while (temperature > minimum_temperature && iteration < maxIterations && currentCost != graph.tsp_optimal_weight && std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < exec_time);

    return bestSolution;
}
