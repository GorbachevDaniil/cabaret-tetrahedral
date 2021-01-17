#ifndef Cell_hpp
#define Cell_hpp

#include <vector>
#include <map>

#include <Eigen/Dense>

class Mesh;

class Cell {
   public:
    long ID;
    long centerNodeID;
    double volume;

    std::vector<long> cornerNodeIDs;
    std::vector<long> faceIDs;

    std::map<long, int> faceToNormalDir;
    std::map<long, long> nodeIDToOppositeNodeID;

    Cell(long ID, long nodeID1, long nodeID2, long nodeID3, long nodeID4);
};

#endif
