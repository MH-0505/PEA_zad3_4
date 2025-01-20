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
#include "../TsplibParser.h"

std::string path_to_string(std::vector<int> results);
int checkFileExtension(const std::string& filename);

int main() {
    std::map<std::string, std::string> configuration = ConfigManager::load_configuration("config_sa.txt");
    std::vector<std::string> filenames = ConfigManager::parse_filenames(configuration);
    AdjMatrix graph;
    std::fstream output, output2;
    output.open("output_sa.csv", std::ios::app);

    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time_t);
    output  << "\n\n" << "BADANIE" << ";" << std::put_time(local_time, "%H:%M:%S") << "\n"
            << "metoda:" << ";" << "simulated annealing" << "\n"
            << "konfiguracja:;" << ConfigManager::config_to_string(configuration);
    output.close();

    for(const std::string& filename: filenames){
        int filetype = checkFileExtension(filename);
        if(filetype == 0 ){
            graph.loadGraph(filename);
        }else if(filetype == 1){
            TsplibParser::parseTSPFile(graph, filename);
        }else if(filetype == -1){
            std::cout << "\nBLEDNY PLIK\n";
            continue;
        }

        std::cout << "\n\nROZPOCZETO BADANIE\nMetoda: simulated annealing\nNazwa pliku: " << filename << "\nWynik optymalny: " << graph.tsp_optimal_weight;
        output.open("output_sa.csv", std::ios::app);
        output2.open("output_sa_2.csv", std::ios::app);
        output << "\n\nplik:;" << filename << "\n"
               << "wynik optymalny:;" << graph.tsp_optimal_weight << "\n"
               << "liczba wierzcholkow:;" << graph.vertex_count << "\n"
               << "iteracja;czas [ms];wynik;blad bezwzgledny;blad wzgledny;sciezka\n";

        output2 << std::put_time(local_time, "%H:%M:%S") << ";";

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
            auto time = std::chrono::duration_cast<std::chrono::milliseconds >(end_time-start_time).count();

            int absolute_error = std::abs(graph.tsp_optimal_weight - results[0]);
            double relative_error = static_cast<double>(absolute_error)/ graph.tsp_optimal_weight * 100.0;
            error_sum += absolute_error;
            avg_time += time;

            std::stringstream ss;
            ss << std::fixed << std::setprecision(2) << relative_error;
            std::string error_string = ss.str() + '%';

            std::cout << "\n\nIteracja: " << i << "\nCzas realizacji: " << time << " ms\nWynik algorytmu: " << results[0]
                      << "\nBlad bezwzgledny: " << absolute_error << "\nBlad wzgledny: " << error_string << "\nWyznaczona sciezka: " << path_to_string(results);
            output << i << ";" << time << ";" << results[0] << ";" << absolute_error << ";" <<relative_error << ";" << path_to_string(results)  << "\n";
        }
        avg_time = avg_time/iterations;
        double ua = static_cast<double>(error_sum) / iterations;

        output << "sredni czas [ms];blad bezwzgledny;blad bezwzgledny x2;blad wzgledny [%]\n";
        output << avg_time << ";" << ua << ";" << ua*2 << ";" << 2*ua/graph.tsp_optimal_weight << "\n";
        output2 << filename << ";" << graph.vertex_count << ";" << avg_time << ";" << ua << ";" << ua*2 << ";" << 2*ua/graph.tsp_optimal_weight << "\n";
        output.close();
        output2.close();
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

int checkFileExtension(const std::string& filename) {
    std::string lastThree = filename.substr(filename.size() - 3);
    if(lastThree == "txt") return 0;
    else if(lastThree == "tsp") return 1;
    else return -1;
}
