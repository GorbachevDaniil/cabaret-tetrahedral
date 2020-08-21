#ifndef Node_hpp
#define Node_hpp

#include <set>
#include <vector>

class Mesh;

class Node {
   public:
    long ID;
    long cellID;
    long faceID;
    bool boundNode;
    bool cellCenterNode;

    std::vector<double> coords;

    Node(long ID, double x, double y, double z, bool boundNode, bool cellCenterNode);
    Node(long ID, std::vector<double> coords, bool boundNode, bool cellCenterNode);
};

#endif
