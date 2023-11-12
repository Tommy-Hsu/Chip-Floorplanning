#pragma once

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

class BSTree
{
public:

    /* data */
    int root_ = -1;
    std::unordered_map<int, Node*> nodes; // same id as Block

    /* constructor */
    BSTree(const int nNodes) {
        for (int i = 1; i <= nNodes; i++) {
            Node* node = new Node();
            nodes[i] = node;
        }
    };

    /* destructor */
    ~BSTree() {
        for (auto& node : nodes) {
            delete node.second;
        }
    };

};

class ContourNode 
{
public:
    // x1: x coordinate of left side, x2: x coordinate of right side, y2: y coordinate from bottom side
    int x1, x2, y2;
    ContourNode(int x1, int x2, int y2) : x1(x1), x2(x2), y2(y2) {};
    ~ContourNode() {};
};