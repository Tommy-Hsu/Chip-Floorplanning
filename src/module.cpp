#include "module.h"

void BSTree::randomize_initial_bstree(){
    const int nNodes = nBlocks_;
    std::vector<int> ids = generate_random_ids();

    /* fully binary tree */
    root_ = ids[0];
    nodes[root_]->left_ = ids[1];
    nodes[root_]->right_ = ids[2];

    for(int i = 1; i < nNodes; i++){
        nodes[ids[i]]->parent_ = ids[(i-1)/2];
        if(2*i+1 < nNodes) nodes[ids[i]]->left_ = ids[2*i+1];
        if(2*i+2 < nNodes) nodes[ids[i]]->right_ = ids[2*i+2];
    }

    std::cout << " -------- BSTree Randomize Done --------\n";
    #ifdef DEBUG_FLAG
        display_bstree();
    #endif
}

std::vector<int> BSTree::generate_random_ids(){
    const int nNodes = nBlocks_;
    std::vector<int> ids;
    for(int i = 1; i <= nNodes; i++){
        ids.push_back(i);
    }
    std::random_shuffle(ids.begin(), ids.end());
    return ids;
}

void BSTree::packing_bstree(){
    W_ = 0;
    H_ = 0;

    /* self-defined binary tree */
    // root_ = 2;
    // nodes[1]->left_ = -1; nodes[1]->right_ = -1; nodes[1]->parent_ = 2;
    // nodes[2]->left_ = 3; nodes[2]->right_ = 1; nodes[2]->parent_ = -1; blocks_[2]->rotate();
    // nodes[3]->left_ = -1; nodes[3]->right_ = 4; nodes[3]->parent_ = 2; blocks_[3]->rotate();
    // nodes[4]->left_ = -1; nodes[4]->right_ = -1; nodes[4]->parent_ = 3;

    // #ifdef DEBUG_FLAG
    //     display_bstree();
    // #endif

    int root = root_;
    contour_line_.clear();
    dfs_preorder(root);
}

void BSTree::dfs_preorder(int node_id){
    if(node_id > 0){
        const auto& curr_block_ptr = blocks_[node_id];
        int parent_id = nodes[node_id]->parent_;
        const auto& parent_block_ptr = blocks_[parent_id];

        if(parent_id == -1){
            curr_block_ptr->set_x(0);
            curr_block_ptr->set_y(0);
            ContourNode cn(0, curr_block_ptr->get_width(), curr_block_ptr->get_height());
            contour_line_.push_back(cn);
            W_ = curr_block_ptr->get_width();
            H_ = curr_block_ptr->get_height();
        }
        else if(node_id == nodes[parent_id]->left_){

            // x coordinate
            curr_block_ptr->set_x(parent_block_ptr->get_x() + parent_block_ptr->get_width());
            if( curr_block_ptr->get_x() + curr_block_ptr->get_width() > W_ ){
                W_ = curr_block_ptr->get_x() + curr_block_ptr->get_width();
            }
            // y coordinate
            curr_block_ptr->set_y(update_contour_line(node_id));
        }
        else if(node_id == nodes[parent_id]->right_){
            
            // x coordinate
            curr_block_ptr->set_x(parent_block_ptr->get_x());
            if( curr_block_ptr->get_x() + curr_block_ptr->get_width() > W_ ){
                W_ = curr_block_ptr->get_x() + curr_block_ptr->get_width();
            }
            // y coordinate
            curr_block_ptr->set_y(update_contour_line(node_id));
        }

        dfs_preorder(nodes[node_id]->left_);
        dfs_preorder(nodes[node_id]->right_);
    
    }
}

int BSTree::update_contour_line(int node_id){

    // x1: left, y1: bottom, x2: right , y2: top
    int nd_x1 = blocks_[node_id]->get_x();
    int nd_x2 = nd_x1 + blocks_[node_id]->get_width();
    int nd_y1{0}, nd_y2{0}; 
    
    auto it = contour_line_.begin();
    /* could we iterator from parent node x coordinate !? */
    while (it != contour_line_.end()){
        auto& cn = *it;
        if(cn.x2 <= nd_x1){
            it++;
        }
        else if(nd_x2 <= cn.x1){
            break;
        }
        else{
            nd_y1 = std::max(cn.y2, nd_y1);
            if(nd_x1 <= cn.x1 && cn.x2 <= nd_x2){
                /* Case 1: nd_x1 <= cn.x1 and cn.x2 <= nd_x2 */
                // erase will return next iterator
                it = contour_line_.erase(it);
            }
            else if (cn.x1 <= nd_x1 && cn.x2 <= nd_x2){
                /* Case 2: cn.x1 <= nd_x1 and cn.x2 <= nd_x2 */
                cn.x2 = nd_x1;
            }
            else if (nd_x1 <= cn.x1 && nd_x2 <= cn.x2){
                /* Case 3: nd_x1 <= cn.x1 and nd_x2 <= cn.x2 */
                cn.x1 = nd_x2;
            }
            else if (cn.x1 <= nd_x1 && nd_x2 <= cn.x2){
                /* Case 4: cn.x1 <= nd_x1 and nd_x2 <= cn.x2 */
                ContourNode new_cn(cn.x1, nd_x1, cn.y2);
                cn.x1 = nd_x2;
                contour_line_.insert(it, new_cn);
            }
        }
    }

    nd_y2 = nd_y1 + blocks_[node_id]->get_height();
    ContourNode new_cn(nd_x1, nd_x2, nd_y2);
    contour_line_.insert(it, new_cn);
    H_ = std::max(H_, nd_y2);

    return nd_y1;
}

void BSTree::swap_nodes(int nn1, int nn2){

    auto n1 = *(nodes[nn1]);
    auto n2 = *(nodes[nn2]);

    if(n1.left_!= -1){  
        nodes[n1.left_]->parent_  = nn2;
    }
    if(n1.right_!= -1){
        nodes[n1.right_]->parent_ = nn2;  
    }
    if(n1.parent_ != -1){
        if(nodes[n1.parent_]->left_==nn1)
            nodes[n1.parent_]->left_ = nn2;
        else
            nodes[n1.parent_]->right_ = nn2; 
    }else{
        root_ = nn2;
    }

    if(n2.left_!=-1){
        nodes[n2.left_]->parent_  = nn1;
    }
    if(n2.right_!=-1){
        nodes[n2.right_]->parent_ = nn1;  
    }
    if(n2.parent_ != -1){
        if(nodes[n2.parent_]->left_==nn2)
            nodes[n2.parent_]->left_  = nn1;
        else
            nodes[n2.parent_]->right_ = nn1; 
    }else{
        root_ = nn1;
    }

    std::swap(nodes[nn1]->left_,nodes[nn2]->left_);
    std::swap(nodes[nn1]->right_,nodes[nn2]->right_);
    std::swap(nodes[nn1]->parent_,nodes[nn2]->parent_);
}

void BSTree::delete_node(int nn){
    int parent_id = nodes[nn]->parent_;
    int left_id   = nodes[nn]->left_;
    int right_id  = nodes[nn]->right_;

    if( left_id != -1 && right_id != -1 ){
        /* two children */
        bool mv_left;
        do{
            if( nodes[nn]->left_ != -1 && nodes[nn]->right_ != -1 ){
                mv_left = rand()%2;
            }else if( nodes[nn]->left_ != -1 ){
                mv_left = true;
            }else{
                mv_left = false;
            }
            
            swap_nodes(nn, (mv_left) ? nodes[nn]->left_ : nodes[nn]->right_);

        }while( nodes[nn]->left_ != -1 || nodes[nn]->right_ != -1);

        parent_id = nodes[nn]->parent_;
        if( nodes[parent_id]->left_ == nn ){
            nodes[parent_id]->left_ = -1;
        }else{
            nodes[parent_id]->right_ = -1;
        }
    }else if( left_id != -1 || right_id != -1){
        /* one child */
        int child_id = (left_id != -1) ? left_id : right_id;
        if(parent_id != -1){
            if(nodes[parent_id]->left_ == nn){
                nodes[parent_id]->left_ = child_id;
            }else{
                nodes[parent_id]->right_ = child_id;
            }
        }else{
            root_ = child_id;
        }
        nodes[child_id]->parent_ = parent_id;

    }else{
        /* no child */
        if(nodes[parent_id]->left_ == nn){
            nodes[parent_id]->left_ = -1;
        }else{
            nodes[parent_id]->right_ = -1;
        }
    }

    nodes[nn]->parent_ = -1;
    nodes[nn]->left_   = -1;
    nodes[nn]->right_  = -1;
}

void BSTree::insert_node(int nn, int nnp){

    bool pick_right = rand()%2;
    if(pick_right){
        nodes[nn]->right_ = nodes[nnp]->right_;
        nodes[nn]->left_ = -1;
        if(nodes[nnp]->right_ != -1)
            nodes[nodes[nnp]->right_]->parent_ = nn;
        nodes[nnp]->right_ = nn;
    }else{
        nodes[nn]->left_ = nodes[nnp]->left_;
        nodes[nn]->right_ = -1;
        if(nodes[nnp]->left_ != -1)
            nodes[nodes[nnp]->left_]->parent_ = nn;
        nodes[nnp]->left_ = nn;
    }

    nodes[nn]->parent_ = nnp;
}

void BSTree::perturb(){
    int m, n, t;
    m = rand()%3 + 1;
    n = rand()%nBlocks_ + 1;

    if( m == 1 ){
        /* rotate */
        blocks_[n]->rotate();
    }
    else if( m == 2 ){
       /* delete & insert */
        do{
            t = rand()%nBlocks_ + 1;
        }while(t == n);
        delete_node(n);
        insert_node(n, t);
    }
    else if( m == 3 ){
        /* swap 2 nodes */
        do{
            t = rand()%nBlocks_ + 1;
        }while(t == n|| t == nodes[n]->parent_ || n == nodes[t]->parent_);
        swap_nodes(n, t);
    }

    std::cout << " -------- BSTree Perturb Done --------\n";
    std::cout << "m: " << m << ", n: " << n << ", p: " << t << std::endl;
    #ifdef DEBUG_FLAG
        display_bstree();
    #endif
}

/* display */
void BSTree::display_bstree(){

    printf("Root: %d\n", root_);
    int nNodes = nodes.size();
    for(int i = 1; i <= nNodes; i++){
        printf("Node %d: left %d, right %d, parent %d\n", i, nodes[i]->left_, nodes[i]->right_, nodes[i]->parent_);
    }
    
}