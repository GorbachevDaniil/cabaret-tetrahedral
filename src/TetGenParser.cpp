#include "TetGenParser.hpp"

#include "Mesh.hpp"
 
#include <fstream>
#include <iostream>

int TetGenParser::LoadNodes(Mesh *mesh, std::string filename) {
    std::string line;
    std::ifstream nodeFile(filename);

    if (!nodeFile) {
        std::cout << "Error opening node file " << filename << std::endl;
        return 1;
    }

    long numberOfNodes = 0;
    int dimension = 0;
    int numberOfAttributes = 0;
    int boundaryMarker = 0;
    nodeFile >> numberOfNodes >> dimension >> numberOfAttributes >> boundaryMarker;

    int x = 0;
    int y = 0;
    int z = 0;
    long nodeID = 0;
    while (nodeID < numberOfNodes - 1) {
        nodeFile >> nodeID >> x >> y >> z;
        Node *node = new Node(nodeID, x, y, z, false, false);
        mesh->nodes.push_back(*node);
    }

    return 0;
}

int TetGenParser::LoadFaces(Mesh *mesh, std::string filename) {
    std::string line;
    std::ifstream faceFile(filename);

    if (!faceFile) {
        std::cout << "Error opening face file " << filename << std::endl;
        return 1;
    }

    int numberOfFaces = 0;
    int boundaryMarker = 0;
    faceFile >> numberOfFaces >> boundaryMarker;

    long faceID = 0;
    long nodeID1 = 0;
    long nodeID2 = 0;
    long nodeID3 = 0;
    bool isBound = false;
    while (faceID < numberOfFaces - 1) {
        faceFile >> faceID >> nodeID1 >> nodeID2 >> nodeID3 >> boundaryMarker;
        isBound = boundaryMarker != 0;
        Face *face = new Face(faceID, nodeID1, nodeID2, nodeID3, isBound);
        mesh->nodes[nodeID1].boundNode = isBound;
        mesh->nodes[nodeID2].boundNode = isBound;
        mesh->nodes[nodeID3].boundNode = isBound;
        mesh->faces.push_back(*face);
    }

    return 0;
}

int TetGenParser::LoadCells(Mesh *mesh, std::string filename) {
    std::string line;
    std::ifstream cellFile(filename);

    if (!cellFile) {
        std::cout << "Error opening ele file " << filename << std::endl;
        return 1;
    }

    long numberOfCells = 0;
    int numberOfNodes = 0;
    int regionAttribute = 0;
    cellFile >> numberOfCells >>  numberOfNodes >> regionAttribute;

    long cellID = 0;
    long nodeID1 = 0;
    long nodeID2 = 0;
    long nodeID3 = 0;
    long nodeID4 = 0;
    while (cellID < numberOfCells - 1) {
        cellFile >> cellID >> nodeID1 >> nodeID2 >> nodeID3 >> nodeID4;
        Cell *cell = new Cell(cellID, nodeID1, nodeID2, nodeID3, nodeID4);
        mesh->cells.push_back(*cell);
    }

    return 0;
}