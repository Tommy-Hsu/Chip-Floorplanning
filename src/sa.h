#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <unordered_map>
#include "module.h"

class SimulationAnnealing
{
private:
    double Rlowerbound_;
    double Rupperbound_;
    std::unordered_map<int, Block*> blocks;
    BSTree* best_bstree = nullptr;
    BSTree* tmp_bstree = nullptr;
    int nBlocks_ = 0;
    int W_ = 0;   // width
    int H_ = 0;   // height

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
};