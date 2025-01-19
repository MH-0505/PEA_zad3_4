#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <string>
#include <random>
#include <chrono>
#include "../AdjMatrix.h"
#include "TspBranchAndBound.h"

using namespace std;

const int INF = numeric_limits<int>::max();

int path_cost(std::vector<int> &path, AdjMatrix &graph){
    int cost = 0;
    for(int i = 0; i < graph.vertex_count ; i++){
        cost += graph.matrix[path[i]][path[i+1]];
    }
    return cost;
}

struct Node {
    vector<int> path;
    vector<vector<int>> reducedMatrix;
    int cost;
    int level;
    int vertex_index;

    bool operator<(const Node& other) const {
        return cost > other.cost;
    }
};

vector<vector<int>> initiateCostMatrix(AdjMatrix& graph) {
    int n = graph.vertex_count;
    vector<vector<int>> costMatrix(n, vector<int> (n, 0));

    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph.matrix[i][j] == -1) {
                costMatrix[i][j] = INF;
            } else {
                costMatrix[i][j] = graph.matrix[i][j];
            }
        }
    }
    return costMatrix;
}

int reduceMatrix(vector<vector<int>>& matrix) {
    int reductionCost = 0;

    for (int i = 0; i < matrix.size(); i++) {
        int rowMin = INF;
        for (int j = 0; j < matrix.size(); j++) {
            if (matrix[i][j] < rowMin) rowMin = matrix[i][j];
        }
        if (rowMin != INF && rowMin > 0) {
            reductionCost += rowMin;
            for (int j = 0; j < matrix.size(); j++) {
                if (matrix[i][j] != INF) matrix[i][j] -= rowMin;
            }
        }
    }

    for (int j = 0; j < matrix.size(); j++) {
        int colMin = INF;
        for (int i = 0; i < matrix.size(); i++) {
            if (matrix[i][j] < colMin) colMin = matrix[i][j];
        }
        if (colMin != INF && colMin > 0) {
            reductionCost += colMin;
            for (int i = 0; i < matrix.size(); i++) {
                if (matrix[i][j] != INF) matrix[i][j] -= colMin;
            }
        }
    }

    return reductionCost;
}

std::vector<int> TspBranchAndBound::start_algorithm(AdjMatrix& graph, int exec_time, int starting_vertex, SearchType searchType) {
    std::vector<int> results;
    vector<vector<int>> costMatrix = initiateCostMatrix(graph);

    int n = graph.vertex_count;
    int bestCost = INF;
    vector<int> bestPath;

    queue<Node> bfsQueue;
    stack<Node> dfsStack;
    priority_queue<Node> priorityQueue;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> range(0, n - 1);
    

    Node root;
    if(starting_vertex == -1){
        starting_vertex = range(gen);
    } else if(starting_vertex > graph.vertex_count - 1){
        starting_vertex = n-1;
    }
    root.path = {starting_vertex};
    root.reducedMatrix = costMatrix;
    root.cost = reduceMatrix(root.reducedMatrix);
    root.level = 0;
    root.vertex_index = starting_vertex;

    if (searchType == BFS) {
        bfsQueue.push(root);
    } else if (searchType == DFS) {
        dfsStack.push(root);
    } else if (searchType == LOWEST_COST) {
        priorityQueue.push(root);
    }

    auto start = std::chrono::steady_clock::now();

    while (((!bfsQueue.empty() && searchType == BFS) ||
           (!dfsStack.empty() && searchType == DFS) ||
           (!priorityQueue.empty() && searchType == LOWEST_COST)) &&
            (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < exec_time)) {

        Node current;
        switch (searchType) {
            case DFS:
                current = dfsStack.top();
                dfsStack.pop();
                break;
            case BFS:
                current = bfsQueue.front();
                bfsQueue.pop();
                break;
            case LOWEST_COST:
                current = priorityQueue.top();
                priorityQueue.pop();
                break;
        }

        if (current.level == n - 1) {
            current.path.push_back(root.vertex_index);
            //int finalCost = current.cost + root.reducedMatrix[current.vertex_index][root.vertex_index];
            int finalCost = path_cost(current.path, graph);
            if (finalCost < bestCost) {
                bestCost = finalCost;
                bestPath = current.path;
            }
            continue;
        }

        for (int next_vertex = 0; next_vertex < n; next_vertex++) {
            if (find(current.path.begin(), current.path.end(), next_vertex) == current.path.end()) {
                Node child;
                child.path = current.path;
                child.path.push_back(next_vertex);
                child.reducedMatrix = current.reducedMatrix;

                for (int i = 0; i < n; i++) {
                    child.reducedMatrix[current.vertex_index][i] = INF;
                    child.reducedMatrix[i][next_vertex] = INF;
                }
                child.reducedMatrix[next_vertex][root.vertex_index] = INF;

                child.cost = current.cost + current.reducedMatrix[current.vertex_index][next_vertex] + reduceMatrix(child.reducedMatrix);
                child.level = current.level + 1;
                child.vertex_index = next_vertex;

                if (child.cost < bestCost) {
                    switch (searchType) {
                        case DFS:
                            dfsStack.push(child);
                            break;
                        case BFS:
                            bfsQueue.push(child);
                            break;
                        case LOWEST_COST:
                            priorityQueue.push(child);
                            break;
                    }
                }
            }
        }
    }

    results.push_back(bestCost);
    for (int i = 0; i < n; i++) {
        results.push_back(bestPath[i]);
    }

    return results;
}


