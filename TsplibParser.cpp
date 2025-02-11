#include "TsplibParser.h"
#include "AdjMatrix.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>
std::map<std::string, int> optimal_weigths = {{"asy/br17.atsp", 39}, {"asy/ftv33.atsp", 1286}, {"asy/ftv35.atsp", 1473},
                                              {"asy/ftv38.atsp", 1530}, {"asy/ftv44.atsp", 1613}, {"asy/ftv47.atsp", 1776},
                                              {"sym/bays29.tsp", 2020}, {"sym/dantzig42.tsp", 699}, {"sym/fri26.tsp", 937},
                                              {"sym/gr17.tsp", 2085}, {"sym/gr21.tsp", 2707}, {"sym/gr24.tsp", 1272},
                                              {"sym/gr48.tsp", 5046}};

void TsplibParser::parseTSPFile(AdjMatrix& graph, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file");
    }

    std::string line;
    int dimension = 0;
    std::string edgeWeightType;
    std::string edgeWeightFormat;
    bool readingEdgeWeights = false;
    graph.tsp_optimal_weight = optimal_weigths[filename];

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "DIMENSION:") {
            iss >> dimension;
            graph.initiateMatrix(dimension);
        } else if (key == "EDGE_WEIGHT_TYPE:") {
            iss >> edgeWeightType;
        } else if (key == "EDGE_WEIGHT_FORMAT:") {
            iss >> edgeWeightFormat;
        } else if (key == "EDGE_WEIGHT_SECTION") {
            readingEdgeWeights = true;


            std::vector<int> weights;

            // Read weights
            while (std::getline(file, line)) {
                std::istringstream weightStream(line);
                int weight;
                while (weightStream >> weight) {
                    weights.push_back(weight);
                }
            }

            if (edgeWeightFormat == "FULL_MATRIX") {

                int count = 0;
                for (int i = 0; i < dimension; ++i) {
                    for (int j = 0; j < dimension; ++j) {
                        graph.matrix[i][j] = weights[count++];
                    }
                }

            } else if (edgeWeightFormat == "LOWER_DIAG_ROW") {
                int count = 0;

                for (int i = 0; i < dimension; ++i) {
                    for (int j = 0; j <= i; ++j) {

                        graph.matrix[i][j] = weights[count];
                        graph.matrix[j][i] = weights[count];
                        ++count;
                    }
                }
            } else {
                throw std::runtime_error("Unsupported EDGE_WEIGHT_FORMAT.");
            }
        } else if (key == "EOF") {
            break;
        }
    }

    file.close();

    if (!readingEdgeWeights || edgeWeightType != "EXPLICIT") {
        throw std::runtime_error("File does not contain a valid EDGE_WEIGHT_SECTION or is unsupported.");
    }
}


