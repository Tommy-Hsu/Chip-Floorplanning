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
        b_id = static_cast<int> (std::stoi(word.substr(1)));
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
    best_bstree = new BSTree(nBlocks_, blocks);
    curr_bstree = new BSTree(nBlocks_, blocks);

    tmp_bstree->randomize_initial_bstree();
    
    normalized_cost_(tmp_bstree, 10);
    

    *best_bstree = *tmp_bstree;   
    best_cost_ = calculate_cost(best_bstree);
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
        output << "b" << i << " " << blocks[i]->get_x() << " " << blocks[i]->get_y() << " ";
        if(blocks[i]->get_is_rotate()){
            output << "R";
        }
        output << std::endl;
    }
    std::cout << " -------- Output Done --------\n";
    #ifdef DEBUG_FLAG
        display_blocks();
    #endif
}

/* function */
void SimulationAnnealing::normalized_cost_(BSTree* bstree, int t){
    
    for(int i = 0; i < t; i++){
        bstree->perturb();
        bstree->packing_bstree();
        area_norm_ += bstree->W_ * bstree->H_;
        ratio_norm_ += (double)bstree->W_ / (double)bstree->H_;

        W_ = (bstree->W_ < W_)? bstree->W_:W_;
        H_ = (bstree->H_ < H_)? bstree->H_:H_;
    }

    area_norm_ /= t;
    ratio_norm_ /= t;
    std::cout << "area_norm_: " << area_norm_ << " ratio_norm_: " << ratio_norm_ << std::endl;
}

double SimulationAnnealing::calculate_cost(BSTree* bstree){
    
    double cost = 0.0;
    int area = bstree->W_ * bstree->H_;
    double ratio = (double)bstree->W_ / (double)bstree->H_;
    // double width_penalty = (bstree->W_ > W_)? (bstree->W_/W_):0.0;
    // double height_penalty = (bstree->H_ > H_)? (bstree->H_/H_):0.0;

    cost = alpha_ * (double)area / area_norm_
        + (1 - alpha_) * (ratio/ratio_norm_ - Rupperbound_) * (ratio/ratio_norm_ - Rlowerbound_)
        // + width_penalty + height_penalty
        ;

    std::cout<<"area/area_norm_: " << (double)area/area_norm_ 
        <<"\n (Ru-r)(Rl-r): "<< (Rupperbound_ - ratio) * (Rlowerbound_ - ratio) << std::endl;
        // <<"\n width_penalty: "<<width_penalty
        // <<"\n height_penalty: "<<height_penalty<<std::endl;

    return cost;
}

/* display */
void SimulationAnnealing::display_blocks(){

    printf("Rlowerbound: %f, Rupperbound: %f\n", Rlowerbound_, Rupperbound_);
    for(int i = 1; i <= nBlocks_; i++){
        printf("B%d (%d, %d) %d\n", i, blocks[i]->get_x(), blocks[i]->get_y(), blocks[i]->get_is_rotate());
    }
}