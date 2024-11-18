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

// Struktura reprezentująca węzeł drzewa przeszukiwania
struct Node {
    vector<int> path;       // Odwiedzona ścieżka (kolejność miast)
    vector<vector<int>> reducedMatrix; // Zredukowana macierz kosztów
    int cost;               // Koszt częściowej ścieżki
    int level;              // Poziom węzła (liczba odwiedzonych miast)
    int city;               // Ostatnio odwiedzone miasto

    // Operator porównania do kolejki priorytetowej (dla najmniejszego kosztu)
    bool operator<(const Node& other) const {
        return cost > other.cost;
    }
};

// Funkcja pomocnicza do przetwarzania macierzy sąsiedztwa
vector<vector<int>> initiateCostMatrix(AdjMatrix& graph) {
    int n = graph.vertex_count;
    vector<vector<int>> costMatrix(n, vector<int> (n, 0));

    for(int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph.matrix[i][j] == -1 || graph.matrix[i][j] == 0) {
                costMatrix[i][j] = INF;     // Brak krawędzi oznaczony jako INF
            } else {
                costMatrix[i][j] = graph.matrix[i][j];
            }
        }
    }
    return costMatrix;
}

// Funkcja pomocnicza do redukcji macierzy kosztów
int reduceMatrix(vector<vector<int>>& matrix) {
    int reductionCost = 0;

    // Redukcja wierszy
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

    // Redukcja kolumn
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

// Funkcja Branch and Bound do rozwiązania problemu TSP
std::vector<int> TspBranchAndBound::start_algorithm(AdjMatrix& graph, int exec_time, int starting_vertex, SearchType searchType) {
    std::vector<int> results;
    vector<vector<int>> costMatrix = initiateCostMatrix(graph); // Przetwarzamy macierz, zamieniając -1 na INF

    int n = graph.vertex_count;
    int bestCost = INF;
    vector<int> bestPath;

    // Struktury danych do różnych metod przeszukiwania
    queue<Node> bfsQueue;                // Kolejka FIFO dla BFS
    stack<Node> dfsStack;                // Stos LIFO dla DFS
    priority_queue<Node> priorityQueue; // Kolejka priorytetowa dla Lowest Cost First

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> range(0, n - 1);
    
    // Tworzenie początkowego węzła
    Node root;
    if(starting_vertex == -1){
        root.path = {range(gen)};
    } else if(starting_vertex > graph.vertex_count - 1){
        root.path = {n - 1};
    } else{
        root.path = {starting_vertex};
    }
    root.reducedMatrix = costMatrix;
    root.cost = reduceMatrix(root.reducedMatrix);
    root.level = 0;
    root.city = 0;

    // Wstawienie węzła początkowego do odpowiedniej struktury
    if (searchType == BFS) {
        bfsQueue.push(root);
    } else if (searchType == DFS) {
        dfsStack.push(root);
    } else if (searchType == LOWEST_COST) {
        priorityQueue.push(root);
    }

    auto start = std::chrono::steady_clock::now();
    // Przeszukiwanie
    while (((!bfsQueue.empty() && searchType == BFS) ||
           (!dfsStack.empty() && searchType == DFS) ||
           (!priorityQueue.empty() && searchType == LOWEST_COST)) &&
            (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < exec_time)) {

        // Wybór węzła w zależności od metody przeszukiwania
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

        // Jeśli osiągnięto pełną ścieżkę (wszystkie miasta odwiedzone)
        if (current.level == n - 1) {
            current.path.push_back(0); // Powrót do miasta początkowego
            int finalCost = current.cost + costMatrix[current.city][starting_vertex];
            if (finalCost < bestCost) {
                bestCost = finalCost;
                bestPath = current.path;
            }
            continue;
        }

        // Generowanie dzieci (kolejnych miast do odwiedzenia)
        for (int nextCity = 0; nextCity < n; nextCity++) {
            if (find(current.path.begin(), current.path.end(), nextCity) == current.path.end()) {
                Node child;
                child.path = current.path;
                child.path.push_back(nextCity);
                child.reducedMatrix = current.reducedMatrix;

                // Aktualizacja macierzy kosztów
                for (int i = 0; i < n; i++) {
                    child.reducedMatrix[current.city][i] = INF; // Odcinamy wiersz
                    child.reducedMatrix[i][nextCity] = INF;     // Odcinamy kolumnę
                }
                child.reducedMatrix[nextCity][starting_vertex] = INF;         // Odcinamy powrót do miasta początkowego

                // Obliczenie kosztu
                child.cost = current.cost + costMatrix[current.city][nextCity] + reduceMatrix(child.reducedMatrix);   // TU COS POJEBANEGO
                child.level = current.level + 1;
                child.city = nextCity;

                // Dodanie dziecka do odpowiedniej struktury
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

