#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <iomanip>
#include <fstream>
#include "../AdjMatrix.h"
#include "TpsBruteForce.h"
#include "../ConfigManager.h"

std::string path_to_string(std::vector<int> results);

int main() {
    std::map<std::string, std::string> configuration = ConfigManager::load_configuration("config.txt");
    std::vector<std::string> filenames = ConfigManager::parse_filenames(configuration);
    AdjMatrix graph;
    std::fstream output;
    output.open("output_bf.csv", std::ios::app);
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time_t);
    output  << "\n\n" << "BADANIE" << ";" << std::put_time(local_time, "%H:%M:%S") << "\n"
            << "metoda:" << ";" << "brute-force" << "\n"
            << "konfiguracja:;" << ConfigManager::config_to_string(configuration);
    output.close();

    for(const std::string& filename: filenames){
        graph.loadGraph(filename);
        std::string optimal_s;
        if(graph.tsp_optimal_weight == -1){
            optimal_s = "nieznany";
        } else{
            optimal_s = std::to_string(graph.tsp_optimal_weight);
        }
        std::cout << "\n\nROZPOCZETO BADANIE\nMetoda: brute-force\nNazwa pliku: " << filename << "\nWynik optymalny: " << optimal_s;
        output.open("output_bf.csv", std::ios::app);

        output << "\n\nplik:;" << filename << "\n"
               << "wynik optymalny:;" << graph.tsp_optimal_weight << "\n"
               << "liczba wierzcholkow:;" << graph.vertex_count << "\n"
               << "iteracja;czas [ms];wynik;sciezka\n";

        long long avg_time = 0;
        int iterations = std::stoi(configuration["iterations"]);

        for(int i = 1; i <= iterations; i++){
            auto start_time = std::chrono::high_resolution_clock::now();
            std::vector<int> results = TpsBruteForce::start_algorithm(graph, std::stoi(configuration["max_exec_time_s"]));

            auto end_time = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time).count();

            avg_time += time;

            std::cout << "\n\nIteracja: " << i << "\nCzas realizacji: " << time << " ms\nWynik algorytmu: " << results[0] << "\nWyznaczona sciezka: " << path_to_string(results);
            output << i << ";" << time << ";" << results[0] << ";" << path_to_string(results)  << "\n";
        }
        avg_time = avg_time/iterations;

        output << "sredni czas [ms]\n";
        output << avg_time << "\n";
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
