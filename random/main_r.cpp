#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <iomanip>
#include <fstream>
#include "../AdjMatrix.h"
#include "../ConfigManager.h"
#include "TpsRandom.h"
#include "../brute_force/TpsBruteForce.h"

std::string path_to_string(std::vector<int> results);

int main() {
    std::map<std::string, std::string> configuration = ConfigManager::load_configuration("config.txt");
    std::vector<std::string> filenames = ConfigManager::parse_filenames(configuration);
    AdjMatrix graph;
    std::fstream output;
    output.open(configuration["output_file"], std::ios::app);
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time_t);
    output  << "\n\n" << "BADANIE" << ";" << std::put_time(local_time, "%H:%M:%S") << "\n"
            << "metoda:" << ";" << "random" << "\n";

    for(const std::string& filename: filenames){
        graph.loadGraph(filename);
        std::cout << "\n\nROZPOCZETO BADANIE\nMetoda: random\nNazwa pliku: " << filename << "\nWynik optymalny: " << graph.tsp_optimal_weight;
        std::vector<int> optimal_results = TpsBruteForce::start_algorithm(graph, std::stoi(configuration["max_exec_time_s"]));

        auto start_time = std::chrono::high_resolution_clock::now();

        std::vector<int> results = TpsRandom::start_algorithm(graph, std::stoi(configuration["max_exec_time_s"]), optimal_results[0]);

        auto end_time = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();

        std::cout << "\nCzas realizacji: " << time << " ms\nWynik algorytmu: " << results[0] << "\nWyznaczonasciezka: " << path_to_string(results);

        output << "\nplik:" << ";" << filename << "\n"
               << "wynik optymalny:" << ";" << graph.tsp_optimal_weight << "\n"
                << "wynik algorytmu:" << ";" << results[0] << "\n"
                << "czas realizacji:" << ";" << time << ";" << "ms\n"
                << "sciezka:" << ";" << path_to_string(results) << "\n";
        graph.deleteMatrix();
    }
}

std::string path_to_string(std::vector<int> results){
    std::string s = "";
    for(int i = 1; i < results.size(); i++){
        s += std::to_string(results[i]);
        s += " -> ";
    }
    s += std::to_string(results[1]);
    return s;
}