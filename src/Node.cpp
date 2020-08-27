#include "Node.hpp"

#include <iostream>

Node::Node(long ID, double x, double y, double z, bool boundNode, bool cellCenterNode) {
    this->ID = ID;
    coords[0] = x;
    coords[1] = y;
    coords[2] = z;
    this->boundNode = boundNode;
    this->cellCenterNode = cellCenterNode;
}

Node::Node(long ID, Eigen::Vector3d coords, bool boundNode, bool cellCenterNode) {
    this->ID = ID;
    this->coords = coords;
    this->boundNode = boundNode;
    this->cellCenterNode = cellCenterNode;
}
