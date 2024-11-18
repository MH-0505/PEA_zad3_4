
#ifndef PEA_ZAD1_TSPBRANCHANDBOUND_H
#define PEA_ZAD1_TSPBRANCHANDBOUND_H


class TspBranchAndBound{
public:
    enum SearchType{
        DFS,
        BFS,
        LOWEST_COST
    };
    static std::vector<int> start_algorithm(AdjMatrix& graph, int exec_time, int starting_vertex, SearchType searchType);

};


#endif //PEA_ZAD1_TSPBRANCHANDBOUND_H
