#include "sa.h"

/* workflow */
void SimulationAnnealing::parse_input(std::ifstream& input){

    std::string line, word;
    int b_id, b_width, b_height;
    
    input >> Rlowerbound_ >> Rupperbound_;

    input.ignore(256, '\n');
    while (getline(input, line)) {
        std::stringstream ss(line);
        ss >> word >> b_width >> b_height;
        b_id = std::stoi(word.substr(1));
        Block* b = new Block(b_id, b_width, b_height);
        blocks[b_id] = b;   // blocks[b_id] is a pointer to Block   
        ss.clear();
    }
    nBlocks_ = blocks.size();
    input.close();
    std::cout << " -------- Parse Done --------\n";
}

void SimulationAnnealing::solve(){

    tmp_bstree = new BSTree(nBlocks_, blocks);
    tmp_bstree->randomize_initial_bstree();
    
    tmp_bstree->perturb();
    
    best_bstree = tmp_bstree;
    // tmp_bstree already packing, TODO: calculate cost
    best_bstree->packing_bstree();
}

void SimulationAnnealing::output(std::ofstream& output){

    W_ = best_bstree->W_;
    H_ = best_bstree->H_;
    for(int i = 1; i <= nBlocks_; i++){
        blocks[i]->set_x(best_bstree->blocks_[i]->get_x());
        blocks[i]->set_y(best_bstree->blocks_[i]->get_y());
        if(best_bstree->blocks_[i]->get_is_rotate()){
            blocks[i]->rotate();
        }
    }

    int area = W_ * H_;
    double ratio = (double)W_ / (double)H_;
    output << "A = " << area << std::endl;
    output << "R = " << ratio << std::endl;
    /* assignment */
    // for(int i = 1; i <= nBlocks_; i++){
    //     output << "b" << i << " " << blocks[i]->get_x() << " " << blocks[i]->get_y() << std::endl;
    // }
    /* print */
    for(int i = 1; i <= nBlocks_; i++){
        output << "b" << i << " " << blocks[i]->get_x() << " " << blocks[i]->get_y() << " " << blocks[i]->get_is_rotate() << std::endl;
    }
    std::cout << " -------- Output Done --------\n";
    #ifdef DEBUG_FLAG
        display_blocks();
    #endif
}

/* display */
void SimulationAnnealing::display_blocks(){

    printf("Rlowerbound: %f, Rupperbound: %f\n", Rlowerbound_, Rupperbound_);
    for(int i = 1; i <= nBlocks_; i++){
        printf("B%d (%d, %d) %d\n", i, blocks[i]->get_x(), blocks[i]->get_y(), blocks[i]->get_is_rotate());
    }
}