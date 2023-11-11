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
    DEBUG_COUT(" -------- Parse Done --------\n");
}

void SimulationAnnealing::randomize_initial_bstree(){

    const int nBlocks = nBlocks_;
    bstree = new BSTree(nBlocks);
    std::vector<int> ids = generate_random_ids(nBlocks);
    bstree->root_ = ids[0];
    bstree->nodes[bstree->root_]->left_ = ids[1];
    bstree->nodes[bstree->root_]->right_ = ids[2];

    for(int i = 1; i < nBlocks; i++){
        bstree->nodes[ids[i]]->parent_ = ids[(i-1)/2];
        if(2*i+1 < nBlocks) bstree->nodes[ids[i]]->left_ = ids[2*i+1];
        if(2*i+2 < nBlocks) bstree->nodes[ids[i]]->right_ = ids[2*i+2];
    }

    DEBUG_COUT(" -------- Randomize Done --------\n");
    display_bstree();
}

void SimulationAnnealing::dfs_preorder(int node_id){
    if(node_id > 0){
        DEBUG_COUT("node_id: " << node_id << std::endl);
        const auto& curr_block_ptr = blocks[node_id];
        int parent_id = bstree->nodes[node_id]->parent_;
        const auto& parent_block_ptr = blocks[parent_id];

        if(parent_id == -1){
            curr_block_ptr->set_x(0);
            curr_block_ptr->set_y(0);
            ContourNode cn(0, curr_block_ptr->get_width(), curr_block_ptr->get_height());
            contour_line_.push_back(cn);
            W_ = curr_block_ptr->get_width();
            H_ = curr_block_ptr->get_height();
        }
        else if(node_id == bstree->nodes[parent_id]->left_){
            curr_block_ptr->set_x(parent_block_ptr->get_x() + parent_block_ptr->get_width());
            if( curr_block_ptr->get_x() + curr_block_ptr->get_width() > W_ ){
                W_ = curr_block_ptr->get_x() + curr_block_ptr->get_width();
            }
            // TODO: find the y coordinate
        }
        else if(node_id == bstree->nodes[parent_id]->right_){
            curr_block_ptr->set_x(parent_block_ptr->get_x());
            if( curr_block_ptr->get_x() + curr_block_ptr->get_width() > W_ ){
                W_ = curr_block_ptr->get_x() + curr_block_ptr->get_width();
            }
            // TODO: find the y coordinate
        }

        dfs_preorder(bstree->nodes[node_id]->left_);
        dfs_preorder(bstree->nodes[node_id]->right_);
    
    }
}

void SimulationAnnealing::packing_bstree(){
    
    W_ = 0;
    H_ = 100;
    const int root = bstree->root_;
    dfs_preorder(root);
}

void SimulationAnnealing::solve(){
    
}

void SimulationAnnealing::output(std::ofstream& output){
    int area = W_ * H_;
    double ratio = (double)W_ / (double)H_;
    output << "A = " << area << std::endl;
    output << "R = " << ratio << std::endl;
    for(int i = 1; i <= nBlocks_; i++){
        output << "b" << i << " " << blocks[i]->get_x() << " " << blocks[i]->get_y() << std::endl;
    }
    DEBUG_COUT(" -------- Output Done --------\n");
}

/* display */
void SimulationAnnealing::display_blocks(){

    printf("Rlowerbound: %f, Rupperbound: %f\n", Rlowerbound_, Rupperbound_);
    for(int i = 1; i < nBlocks_; i++){
        printf("B%d (%d, %d) %d\n", i, blocks[i]->get_x(), blocks[i]->get_y(), blocks[i]->get_is_rotate());
    }
}

void SimulationAnnealing::display_bstree(){

    printf("Root: %d\n", bstree->root_);
    for(int i = 1; i <= nBlocks_; i++){
        printf("Node %d: left %d, right %d, parent %d\n", i, bstree->nodes[i]->left_, bstree->nodes[i]->right_, bstree->nodes[i]->parent_);
    }
    
}

/* function */
std::vector<int> SimulationAnnealing::generate_random_ids(const int nBlocks){
    std::vector<int> ids;
    for(int i = 1; i <= nBlocks; i++){
        ids.push_back(i);
    }
    std::random_shuffle(ids.begin(), ids.end());
    return ids;
}