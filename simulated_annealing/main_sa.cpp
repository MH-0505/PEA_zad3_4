#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "TspSA.h"
#include "../AdjMatrix.h"
#include "../ConfigManager.h"
#include "../brute_force/TpsBruteForce.h"

std::string path_to_string(std::vector<int> results);

int main() {
    std::map<std::string, std::string> configuration = ConfigManager::load_configuration("config_sa.txt");
    std::vector<std::string> filenames = ConfigManager::parse_filenames(configuration);
    AdjMatrix graph;
    std::fstream output;
    output.open("output_sa.csv", std::ios::app);
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time_t);
    output  << "\n\n" << "BADANIE" << ";" << std::put_time(local_time, "%H:%M:%S") << "\n"
            << "metoda:" << ";" << "simulated annealing" << "\n"
            << "konfiguracja:;" << ConfigManager::config_to_string(configuration);
    output.close();

    for(const std::string& filename: filenames){
        graph.loadGraph(filename);
        if(graph.tsp_optimal_weight == -1){
            std::vector<int> v = TpsBruteForce::start_algorithm(graph, std::stoi(configuration["max_exec_time_s"]));
            graph.tsp_optimal_weight = v.front();
        }
        std::cout << "\n\nROZPOCZETO BADANIE\nMetoda: simulated annealing\nNazwa pliku: " << filename << "\nWynik optymalny: " << graph.tsp_optimal_weight;
        output.open("output_sa.csv", std::ios::app);
        output << "\n\nplik:;" << filename << "\n"
               << "wynik optymalny:;" << graph.tsp_optimal_weight << "\n"
               << "liczba wierzcholkow:;" << graph.vertex_count << "\n"
               << "iteracja;czas [us];wynik;blad bezwzgledny;blad wzgledny;sciezka\n";

        long long avg_time = 0;
        int error_sum = 0;
        int iterations = std::stoi(configuration["iterations"]);

        for(int i = 1; i <= iterations; i++){
            auto start_time = std::chrono::high_resolution_clock::now();

            std::vector<int> results = TspSA::startAlgorithm(graph, std::stod(configuration["initial_temperature"]),
                                                             std::stod(configuration["minimum_temperature"]),
                                                             std::stod(configuration["cooling_rate"]),
                                                             std::stoi(configuration["max_exec_time_s"]),
                                                             std::stoi(configuration["sa_iterations"]),
                                                             std::stoi(configuration["epoch"]),
                                                             std::stoi(configuration["annealing_mode"]),
                                                             std::stoi(configuration["solution_gen_mode"]),
                                                             std::stoi(configuration["random_start_path"]));

            auto end_time = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::microseconds >(end_time-start_time).count();

            int absolute_error = std::abs(graph.tsp_optimal_weight - results[0]);
            double relative_error = static_cast<double>(absolute_error)/ graph.tsp_optimal_weight * 100.0;
            error_sum += absolute_error;
            avg_time += time;

            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << relative_error;
            std::string error_string = ss.str() + '%';

            std::cout << "\n\nIteracja: " << i << "\nCzas realizacji: " << time << " us\nWynik algorytmu: " << results[0]
                      << "\nBlad bezwzgledny: " << absolute_error << "\nBlad wzgledny: " << error_string << "\nWyznaczona sciezka: " << path_to_string(results);
            output << i << ";" << time << ";" << results[0] << ";" << absolute_error << ";" <<relative_error << ";" << path_to_string(results)  << "\n";
        }
        avg_time = avg_time/iterations;
        double ua = static_cast<double>(error_sum) / iterations;

        output << "sredni czas [us];blad bezwzgledny;blad bezwzgledny x2;blad wzgledny [%]\n";
        output << avg_time << ";" << ua << ";" << ua*2 << ";" << 2*ua/graph.tsp_optimal_weight * 100.0 << "\n";
        output.close();
        graph.deleteMatrix();
    }

    std::cout << "\n\nKONIEC BADANIA\n";
    std::system("pause");
}

std::string path_to_string(std::vector<int> results){
    if(results.size() == 1 || results.empty()){
        return " ";
    }
    std::string s;
    for(int i = 1; i < results.size(); i++){
        s += std::to_string(results[i]);
        s += " ";
    }
    s += std::to_string(results[1]);
    return s;
}
