#include <fstream>
#include <iostream>
#include <vector>
#include "AdjMatrix.h"



void AdjMatrix::loadGraph(const std::string& filename) {
    std::ifstream ifile;
    ifile.open(filename);
    ifile >> vertex_count;
    ifile >> tsp_optimal_weight;
    initiateMatrix(vertex_count);

    if(tsp_optimal_weight <= 0){
        ifile.close();
        ifile.open(filename);
        ifile >> vertex_count;
        for(int i = 0; i < vertex_count; i++){
            for(int j = 0; j < vertex_count; j++) {
                ifile >> matrix[i][j];
            }
        }
    }else{
        for(int i = 0; i < vertex_count; i++){
            for(int j = 0; j < vertex_count; j++) {
                ifile >> matrix[i][j];
            }
        }
    }


    ifile.close();
}

void AdjMatrix::initiateMatrix(int verts_n) {
    vertex_count = verts_n;
    matrix = new int* [verts_n];
    for (int i = 0; i < verts_n; i++) {
        matrix[i] = new int[verts_n];
    }

    for (int a = 0; a < verts_n; a++) {
        for (int b = 0; b < verts_n; b++) {
            matrix[a][b] = 0;
        }
    }
}

void AdjMatrix::deleteMatrix(){
    for (int i = 0; i < vertex_count; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    tsp_optimal_weight = 0;
    vertex_count = 0;
}


void AdjMatrix::printGraph(){

    std::cout << "     ";
    for (int i = 0; i < vertex_count; i++) {
        std::printf("%3d",i);
    }
    std::cout << "\n";

    std::cout << "    __";
    for (int i = 0; i < vertex_count; i++) {
        std::cout << "___";
    }
    std::cout << "\n";

    for (int a = 0; a < vertex_count; a++) {
        std::printf("%3d |",a);
        for (int b = 0; b < vertex_count; b++) {
            std::printf("%3d",matrix[a][b]);
        }
        std::cout << "\n";
    }
}

/*
void AdjMatrix::generateGraph(int verts_n, int density ) {
    int weight;
    edges_number = verts_n-1;
    vertex_count = verts_n;

    initiateMatrix(vertex_count);
    std::srand((unsigned) time(nullptr));

    for(int i = 1; i < vertex_count; i++) {
        weight = (rand() % 9)+1;
        matrix[i][i-1] = weight;
        matrix[i-1][i] = weight;
    }

    for (int a = 0; a < vertex_count; a++) {
        for (int b = 0; b < vertex_count; b++) {
            if((rand() % 101) <= density-(100/vertex_count) && matrix[a][b] == 0){
                matrix[a][b] = (rand() % 9)+1;
                edges_number++;
            }
        }
    }

    for(int i = 0; i < vertex_count; i++){
        matrix[i][i] = 0;
    }
}

void AdjMatrix::makeUndirected(){
    edges_number = 0;
    for (int a = 0; a < vertex_count; a++) {
        for (int b = a; b < vertex_count; b++) {
            matrix[b][a] = matrix[a][b];

            if(matrix[a][b] != 0) edges_number++;
        }
    }
}
*/
