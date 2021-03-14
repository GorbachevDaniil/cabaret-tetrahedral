#include "core/face.h"

Face::Face(long ID, long nodeID1, long nodeID2, long nodeID3, bool boundFace) {
    this->ID = ID;
    cornerNodeIDs.push_back(nodeID1);
    cornerNodeIDs.push_back(nodeID2);
    cornerNodeIDs.push_back(nodeID3);
    this->boundFace = boundFace;
}
