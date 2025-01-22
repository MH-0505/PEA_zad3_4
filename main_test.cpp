//
// Created by michal_studia on 19.01.2025.
//

#include <string>
#include <iostream>
#include "AdjMatrix.h"
#include "TsplibParser.h"

int main() {
    std::string filename = "sym/bays29.tsp";  // Replace with your file path
    AdjMatrix graph{};

    try {
        TsplibParser::parseTSPFile(graph, filename);

        // Output the distance matrix
        graph.printGraph();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}