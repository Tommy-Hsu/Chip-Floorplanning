#include "sa.h"

SimulationAnnealing::SimulationAnnealing()
{
}

SimulationAnnealing::~SimulationAnnealing()
{
}

// workflow
void SimulationAnnealing::parse_input(std::ifstream& input){

    std::string line, word;
    int b_id, b_width, b_height;
    
    input >> Rlowerbound >> Rupperbound;
    input.ignore(256, '\n');
    while (getline(input, line)) {
        std::stringstream ss(line);
        ss >> word >> b_width >> b_height;
        b_id = std::stoi(word.substr(1));
        Block* b = new Block(b_id, b_width, b_height);
        blocks.push_back(b);
        ss.clear();
    }
    input.close();
}

void SimulationAnnealing::output(std::ofstream& output){
    int area = width * height;
    double ratio = (double) width / (double)height;
    output << "A = " << area << std::endl;
    output << "R = " << ratio << std::endl;
    for(auto b : blocks){
        output << "b" << b->b_id << " " << b->b_x << " " << b->b_y << std::endl;
    }
}

// display
void SimulationAnnealing::display_blocks(){

    printf("Rlowerbound: %f, Rupperbound: %f\n", Rlowerbound, Rupperbound);
    for (auto b : blocks){
        printf("%d %d %d\n", b->b_id, b->b_width, b->b_height);
    }
}