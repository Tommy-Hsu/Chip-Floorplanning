#pragma once
#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <cstdlib> 
#include <ctime>
#include <utility>

class Block
{
private:

    /* data */
    int id_;
    int width_;
    int height_;
    int x_ = 0;
    int y_ = 0;
    bool is_rotate_ = false;

public:
    Block(int id, int width, int height) : id_(id), width_(width), height_(height) {};
    ~Block() {};

    /* getter data */
    int get_id() { return id_; }
    int get_x() { return x_; }
    int get_y() { return y_; }
    bool get_is_rotate() { return is_rotate_; }
    int get_width() { return is_rotate_ ? height_ : width_; }
    int get_height() { return is_rotate_ ? width_ : height_; }

    /* setter data */
    void set_x(int x) { x_ = x; }
    void set_y(int y) { y_ = y; }
    void rotate() { is_rotate_ = !is_rotate_; }

};

class Node
{
public:

    /* node */
    int left_ = -1;
    int right_ = -1;
    int parent_ = -1;

};

class ContourNode 
{
public:
    // x1: x coordinate of left side, x2: x coordinate of right side, y2: y coordinate from bottom side
    int x1, x2, y2;
    ContourNode(int x1, int x2, int y2) : x1(x1), x2(x2), y2(y2) {};
    ~ContourNode() {};
};

class BSTree
{
public:

    /* data */
    int root_ = -1;
    std::unordered_map<int, Block*> blocks_; // same id as Nodes
    std::unordered_map<int, Node*> nodes; // same id as Blocks
    double cost = 0;
    int nBlocks_ = 0;
    int W_ = 0;
    int H_ = 0;

    std::list<ContourNode> contour_line_;

    /* constructor */
    BSTree(const int nNodes, std::unordered_map<int, Block*> blocks) {
        nBlocks_ = nNodes;
        for(int i = 1; i <= nNodes; i++){
            Block* block = new Block(blocks[i]->get_id(), blocks[i]->get_width(), blocks[i]->get_height());
            blocks_[i] = block;
        }
        for (int i = 1; i <= nNodes; i++) {
            Node* node = new Node();
            nodes[i] = node;
        }
    };

    /* destructor */
    ~BSTree() {
        for (auto& block : blocks_) {
            delete block.second;
        }
        for (auto& node : nodes) {
            delete node.second;
        }
    };

    /* function */
    void randomize_initial_bstree();
    std::vector<int> generate_random_ids();
    void packing_bstree();
    void dfs_preorder(int node_id);
    int  update_contour_line(int node_id);
    void swap_nodes(int nn1, int nn2); // nn: node number
    void delete_node(int nn);
    void insert_node(int nn, int nnp); // np: number of node parent
    void perturb();

    /* display */
    void display_bstree();
};