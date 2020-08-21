#include "Node.hpp"

#include <iostream>

Node::Node(long ID, double x, double y, double z, bool boundNode, bool cellCenterNode) {
    this->ID = ID;
    coords.push_back(x);
    coords.push_back(y);
    coords.push_back(z);
    this->boundNode = boundNode;
    this->cellCenterNode = cellCenterNode;
}

Node::Node(long ID, std::vector<double> coords, bool boundNode, bool cellCenterNode) {
    this->ID = ID;
    this->coords = coords;
    this->boundNode = boundNode;
    this->cellCenterNode = cellCenterNode;
}
