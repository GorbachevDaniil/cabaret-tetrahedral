#ifndef Face_hpp
#define Face_hpp

#include <vector>

class Mesh;

class Face {
   public:
    long ID;
    bool boundFace;

    std::vector<long> cornerNodeIDs;
    std::vector<long> innerNodeIDs;
    std::vector<long> cellIDs;

    Face(long ID, long nodeID1, long nodeID2, long nodeID3, bool boundFace);
};

#endif
