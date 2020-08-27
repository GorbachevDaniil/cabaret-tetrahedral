#ifndef Face_hpp
#define Face_hpp

#include <vector>

#include <Eigen/Dense>

class Mesh;

class Face {
   public:
    long ID;
    bool boundFace;
    double square;

    Eigen::Vector3d normal;

    std::vector<long> cornerNodeIDs;
    std::vector<long> innerNodeIDs;
    std::vector<long> cellIDs;

    Face(long ID, long nodeID1, long nodeID2, long nodeID3, bool boundFace);
};

#endif
