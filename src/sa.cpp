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

    prev_bstree = new BSTree(nBlocks_, blocks);
    best_bstree = new BSTree(nBlocks_, blocks);
    curr_bstree = new BSTree(nBlocks_, blocks);

    prev_bstree->randomize_initial_bstree();
    normalized_cost_(prev_bstree, 1000);

    const double r = 0.95; // Cooling rate
    const int k = 40;      // Number of iterations at each temperature
    // const double T0 = 1000;

    double delta_cost;
    double p = 0.95; // Uphill probability
    const double T0 = std::fabs(avg_cost_ / log(p));
    double T = T0;
    int reject;

    calculate_cost(prev_bstree);
    *best_bstree = *prev_bstree;   // copy assignment
    *curr_bstree = *prev_bstree;  // copy assignment
    
    do{
        reject = 0;
        for(size_t i = 0; i < k; i++){
            // std::cout << i;
            curr_bstree->perturb();
            delta_cost = calculate_cost(curr_bstree) - prev_bstree->cost_;
            // std::cout << "delta_cost: " << delta_cost << std::endl;
            // std::cout << "std::exp(-delta_cost/T): " << std::exp(-delta_cost/T) << std::endl;
            p = std::min(1.0, std::exp(-delta_cost/T));
            double r = (double)rand()/RAND_MAX;
            // std::cout << "r: " << r << std::endl;
            if(delta_cost <= 0 || p > r){
                *prev_bstree = *curr_bstree;  // copy assignment
                if(curr_bstree->cost_ < best_bstree->cost_){
                    *best_bstree = *curr_bstree;  // copy assignment
                }
            }else{
                reject++;
                *curr_bstree = *prev_bstree;  // copy assignment
            }
        }
        T *= r;
        std::cout << "\nT: " << T << " reject: " << reject << std::endl;
    }while(reject/k < 0.95 && T > 0.1 );

    // output the result by best_bstree
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

    /* avg uphill cost */
    int uphill_count = 0;
    double uphill_cost = 0.0;
    double prev_cost = calculate_cost(bstree);
    for(int i = 0; i < t; i++){
        bstree->perturb();
        bstree->packing_bstree();
        double delta_cost = calculate_cost(bstree) - prev_cost;
        if(delta_cost > 0){
            uphill_count++;
            uphill_cost += delta_cost;
        }
        prev_cost = bstree->cost_;
    }

    avg_cost_ = uphill_cost / uphill_count;
    std::cout << "avg_cost_: " << avg_cost_ << std::endl;
}

double SimulationAnnealing::calculate_cost(BSTree* bstree){
    /* already packing */
    bstree->packing_bstree();

    double cost = 0.0;
    int area = bstree->W_ * bstree->H_;
    double ratio = (double)bstree->W_ / (double)bstree->H_;
    // double width_penalty = (bstree->W_ > W_)? (bstree->W_/W_):0.0;
    // double height_penalty = (bstree->H_ > H_)? (bstree->H_/H_):0.0;

    cost = alpha_ * (double)area / area_norm_
        + (1 - alpha_) * (ratio/ratio_norm_ - Rupperbound_) * (ratio/ratio_norm_ - Rlowerbound_)
        // + width_penalty + height_penalty
        ;

    // std::cout<<"area/area_norm_: " << (double)area/area_norm_ 
    //     <<"\n (r-Ru)(r-Rl): "<< (ratio/ratio_norm_ - Rupperbound_) * (ratio/ratio_norm_ - Rlowerbound_) << std::endl;
        // <<"\n width_penalty: "<<width_penalty
        // <<"\n height_penalty: "<<height_penalty<<std::endl;
    
    bstree->cost_ = cost;
    return cost;
}

/* display */
void SimulationAnnealing::display_blocks(){

    printf("Rlowerbound: %f, Rupperbound: %f\n", Rlowerbound_, Rupperbound_);
    std::cout << "Area: " << W_ * H_ << std::endl;
    std::cout << "Ratio: " << (double)W_ / (double)H_ << std::endl;
    for(int i = 1; i <= nBlocks_; i++){
        printf("B%d (%d, %d) %d\n", i, blocks[i]->get_x(), blocks[i]->get_y(), blocks[i]->get_is_rotate());
    }
}