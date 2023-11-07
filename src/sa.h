#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Block
{
    int b_id;
    int b_width;
    int b_height;
    int b_x = 0;
    int b_y = 0;

    Block(int id, int width, int height) : b_id(id), b_width(width), b_height(height) {}
};


class SimulationAnnealing
{
private:
    double Rlowerbound;
    double Rupperbound;
    std::vector<Block*> blocks;
    int width = 0;
    int height = 0;

public:
    SimulationAnnealing();
    ~SimulationAnnealing();

    // workflow
    void parse_input(std::ifstream& input);
    void output(std::ofstream& output);

    // display
    void display_blocks();
};