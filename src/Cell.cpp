#include "Cell.hpp"

#include <iostream>

Cell::Cell(long ID, long nodeID1, long nodeID2, long nodeID3, long nodeID4) {
    this->ID = ID;
    cornerNodeIDs.push_back(nodeID1);
    cornerNodeIDs.push_back(nodeID2);
    cornerNodeIDs.push_back(nodeID3);
    cornerNodeIDs.push_back(nodeID4);
    
}
