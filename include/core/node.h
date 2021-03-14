#ifndef Node_hpp
#define Node_hpp

#include <set>
#include <vector>

#include <Eigen/Dense>

class Mesh;

class Node {
   public:
    long ID;
    long cellID;
    long faceID;
    bool boundNode;
    bool cellCenterNode;

    Eigen::Vector3d coords;

    Node(long ID, double x, double y, double z, bool boundNode, bool cellCenterNode);
    Node(long ID, Eigen::Vector3d coords, bool boundNode, bool cellCenterNode);
};

#endif
