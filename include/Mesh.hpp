#ifndef Mesh_hpp
#define Mesh_hpp

#include "Node.hpp"
#include "Face.hpp"
#include "Cell.hpp"

#include <vector>

class Mesh {
   public:

    // list of scalar variables on n layer of time
    std::vector<std::vector<double>> s0;
    // list of scalar variables on n+1/2 layer of time
    std::vector<std::vector<double>> s1;
    // list of scalar variables on n+1 layer of time
    std::vector<std::vector<double>> s2;

    // list of vector variables on n layer of time
    std::vector<std::vector<std::vector<double>>> v0;
    // list of vector variables on n+1/2 layer of time
    std::vector<std::vector<std::vector<double>>> v1;
    // list of vector variables on n+1 layer of time
    std::vector<std::vector<std::vector<double>>> v2;

    std::vector<Node> nodes;
    std::vector<Face> faces;
    std::vector<Cell> cells;

    inline long getNewNodeID() { return nodes.size(); };

    void allocateScalars();
    void allocateVectors();

    void createCellCenterNode();
    void createFaceInnerNodes();

    void fillCellFaceRelation();
    void fillNodeNodeRelation();
};

#endif
