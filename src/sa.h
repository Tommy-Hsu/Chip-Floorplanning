#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include "module.h"

class SimulationAnnealing
{
private:

    double Rlowerbound_;
    double Rupperbound_;
    double alpha_ = 0.85;
    double area_norm_ = 0.0;
    double ratio_norm_ = 0.0;

    std::unordered_map<int, Block*> blocks;
    BSTree* best_bstree = nullptr;
    BSTree* tmp_bstree = nullptr;
    BSTree* curr_bstree = nullptr;
    double best_cost_ = 0.0;
    int nBlocks_ = 0;
    int W_ = INT_MAX;   // width
    int H_ = INT_MAX;   // height

public:
    SimulationAnnealing() {};
    ~SimulationAnnealing()
    {
        for (auto& b : blocks) {
            delete b.second;
        }
    }

    /* workflow */
    void parse_input(std::ifstream& input);
    void solve();
    void output(std::ofstream& output);

    /* display */
    void display_blocks();

    /* function */
    void normalized_cost_(BSTree* bstree, int t);
    double calculate_cost(BSTree* bstree);
};