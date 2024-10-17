//
// Created by Michał on 16.06.2024.
//

#ifndef AIZO_2_ADJMATRIX_H
#define AIZO_2_ADJMATRIX_H
#include <string>


class AdjMatrix{
public:
    int** matrix;
    int vertex_count, tsp_optimal_weight;

    void loadGraph(const std::string& filename);
    void printGraph();
    void generateGraph(int verts_n, int density);
    void initiateMatrix(int verts_n);
    void makeUndirected();

    void deleteMatrix();
};


#endif //AIZO_2_ADJMATRIX_H
