#ifndef Cell_hpp
#define Cell_hpp

#include <vector>
#include <map>

class Mesh;

class Cell {
   public:
    long ID;
    long centerNodeID;

    std::vector<double> cornerNodeIDs;
    std::vector<long> faceIDs;

    std::map<long, long> nodeIDToOppositeNodeID;

    Cell(long ID, long nodeID1, long nodeID2, long nodeID3, long nodeID4);
};

#endif
