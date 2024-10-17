//
// Created by michal on 12.10.2024.
//

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "ConfigManager.h"

std::map<std::string, std::string> ConfigManager::load_configuration(const std::string& filename){
    std::map<std::string, std::string> configuration;
    std::ifstream config_file(filename);
    if (!config_file.is_open()) {
        std::cout << "Błąd: nie można otworzyć pliku " << filename << std::endl;
        return configuration;
    }

    std::string line;
    while (std::getline(config_file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        auto equals_sign_pos = line.find('=');
        if(equals_sign_pos != std::string::npos){
            std::string key = line.substr(0,equals_sign_pos);
            std::string value = line.substr(equals_sign_pos+1);

            configuration[key] = value;
        }
    }
    return configuration;
}

std::vector<std::string> ConfigManager::parse_filenames(std::map<std::string, std::string> &configuration) {
    std::vector<std::string> filenames;
    std::stringstream stream(configuration["input_files"]);
    std::string filename;
    while (!stream.eof()) {
        getline(stream, filename, ',');
        filenames.push_back(filename);
    }
    return filenames;
}