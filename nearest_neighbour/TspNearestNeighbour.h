#ifndef PEA_ZAD1_TSPNEARESTNEIGHBOUR_H
#define PEA_ZAD1_TSPNEARESTNEIGHBOUR_H


class TspNearestNeighbour {
public:
    enum EqualWeightPickOrder{
        FIRST,
        LAST,
        RANDOM,
        CHECK_BRANCHES
    };
    static std::vector<int> start_algorithm(AdjMatrix& graph, int exec_time, int starting_vertex, EqualWeightPickOrder pick_order);

private:
    static std::vector<int> check_branch(AdjMatrix &graph, std::vector<bool> &old_visited_nodes, int next_vertex, int initial_vertex, int nodes_left, int exec_time);
};


#endif //PEA_ZAD1_TSPNEARESTNEIGHBOUR_H
