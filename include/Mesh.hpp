#ifndef Mesh_hpp
#define Mesh_hpp

#include "Node.hpp"
#include "Face.hpp"
#include "Cell.hpp"

#include <vector>

#include <Eigen/Dense>

class Mesh {
   public:


    // list of static scalar variables
    std::vector<std::vector<double>> s;
    // list of scalar variables on n layer of time
    std::vector<std::vector<double>> s0;
    // list of scalar variables on n+1/2 layer of time
    std::vector<std::vector<double>> s1;
    // list of scalar variables on n+1 layer of time
    std::vector<std::vector<double>> s2;

    // list of static vector variables
    std::vector<std::vector<Eigen::Vector3d>> v;
    // list of vector variables on n layer of time
    std::vector<std::vector<Eigen::Vector3d>> v0;
    // list of vector variables on n+1/2 layer of time
    std::vector<std::vector<Eigen::Vector3d>> v1;
    // list of vector variables on n+1 layer of time
    std::vector<std::vector<Eigen::Vector3d>> v2;

    std::vector<Node> nodes;
    std::vector<Face> faces;
    std::vector<Cell> cells;

    inline long getNewNodeID() { return nodes.size(); };

    void createCellCenterNode();
    void createFaceInnerNodes();

    void fillCellFaceRelation();
    void fillNodeNodeRelation();

    void calculateCellVolume();
    void calculateFaceSquare();
    void calculateFaceNormal();
};

#endif
