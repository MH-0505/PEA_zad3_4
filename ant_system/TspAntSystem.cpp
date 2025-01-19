//
// Created by michal_studia on 18.01.2025.
//

#include "TspAntSystem.h"
#include "../AdjMatrix.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <limits>
#include <algorithm>
#include <chrono>


std::vector<int> TspAntSystem::startAlgorithm(AdjMatrix& graph, int exec_time, int num_ants, double alpha, double beta, double evaporation_rate, double pheromone_quantity, int max_iterations, int dorigo_alg) {
    int n = graph.vertex_count;

    // Initialize pheromone levels
    std::vector<std::vector<double>> pheromone(n, std::vector<double>(n, 1.0));

    // Random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> random_double(0.0, 1.0);

    // Best solution tracking
    std::vector<int> best_solution;
    int best_cost = std::numeric_limits<int>::max();

    auto start = std::chrono::steady_clock::now();

    // Start iterations
    for (int iteration = 0; iteration < max_iterations; ++iteration) {
        std::vector<std::vector<int>> ant_solutions(num_ants);
        std::vector<int> ant_costs(num_ants, 0);

        // Each ant constructs a solution
        for (int ant = 0; ant < num_ants; ant++) {
            std::vector<bool> visited(n, false);
            ant_solutions[ant].resize(n);

            // Start from a random city
            int current_city = gen() % n;
            visited[current_city] = true;
            ant_solutions[ant][0] = current_city;

            // Build the tour
            for (int step = 1; step < n; step++) {
                // Compute probabilities for next city
                std::vector<double> probabilities(n, 0.0);
                double total_probability = 0.0;

                for (int next_city = 0; next_city < n; next_city++) {
                    if (!visited[next_city]) {
                        double pheromone_level = pheromone[current_city][next_city];
                        double heuristic_value = 1.0 / (graph.matrix[current_city][next_city] + 1e-6);
                        probabilities[next_city] = std::pow(pheromone_level, alpha) * std::pow(heuristic_value, beta);
                        total_probability += probabilities[next_city];
                    }
                }

                // Normalize probabilities and pick next city
                double rand_value = random_double(gen) * total_probability;
                double cumulative_probability = 0.0;

                for (int next_city = 0; next_city < n; ++next_city) {
                    if (!visited[next_city]) {
                        cumulative_probability += probabilities[next_city];
                        if (cumulative_probability >= rand_value) {
                            if(dorigo_alg == 1) {       // QAS
                                pheromone[current_city][next_city] += pheromone_quantity / graph.matrix[current_city][next_city];
                            }
                            current_city = next_city;
                            break;
                        }
                    }
                }

                visited[current_city] = true;
                ant_solutions[ant][step] = current_city;


            }

            // Calculate cost of the solution
            for (int i = 0; i < n - 1; i++) {
                ant_costs[ant] += graph.matrix[ant_solutions[ant][i]][ant_solutions[ant][i + 1]];
            }
            ant_costs[ant] += graph.matrix[ant_solutions[ant].back()][ant_solutions[ant][0]];

            // Update best solution
            if (ant_costs[ant] < best_cost) {
                best_cost = ant_costs[ant];
                best_solution = ant_solutions[ant];
            }
        }


        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                pheromone[i][j] *= (1.0 - evaporation_rate); // Evaporation
            }
        }

        if(dorigo_alg == 0){        // CAS
            for (int ant = 0; ant < num_ants; ant++) {
                for (int i = 0; i < n - 1; i++) {
                    int from = ant_solutions[ant][i];
                    int to = ant_solutions[ant][i + 1];
                    pheromone[from][to] += pheromone_quantity / ant_costs[ant];
                }
                // Return to start
                int from = ant_solutions[ant].back();
                int to = ant_solutions[ant][0];
                pheromone[from][to] += pheromone_quantity / ant_costs[ant];
            }
        }
        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < exec_time) break;
    }

    std::vector<int> return_v(n+1);
    return_v[0] = best_cost;
    int num = 1;
    for (int v:best_solution) {
        return_v[num] = v;
        num++;
    }

    return return_v;
}

