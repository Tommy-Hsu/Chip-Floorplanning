#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
#include <unordered_map>
#include "module.h"
#include "debug.h"

class SimulationAnnealing
{
private:
    double Rlowerbound_;
    double Rupperbound_;
    std::unordered_map<int, Block*> blocks;
    BSTree* bstree = nullptr;
    int nBlocks_ = 0;
    int W_ = 0;   // width
    int H_ = 0;   // height

    std::list<ContourNode> contour_line_;

public:
    SimulationAnnealing() {};
    ~SimulationAnnealing()
    {
        for (auto& b : blocks) {
            delete b.second;
        }
        delete bstree;
    }

    /* workflow */
    void parse_input(std::ifstream& input);
    void randomize_initial_bstree();
    void packing_bstree();
    void dfs_preorder(int node_id);
    void solve();
    void output(std::ofstream& output);

    /* display */
    void display_blocks();
    void display_bstree();

    /* function */
    std::vector<int> generate_random_ids(const int nBlocks);
};