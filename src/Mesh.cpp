#include "Mesh.hpp"

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cassert>

void Mesh::allocateScalars() {
    s0 = std::vector<std::vector<double>>(nodes.size());
    s1 = std::vector<std::vector<double>>(nodes.size());
    s2 = std::vector<std::vector<double>>(nodes.size());
}

void Mesh::allocateVectors() {
    v0 = std::vector<std::vector<std::vector<double>>>(nodes.size());
    v1 = std::vector<std::vector<std::vector<double>>>(nodes.size());
    v2 = std::vector<std::vector<std::vector<double>>>(nodes.size());
}

std::vector<double> calcNewCoords(std::vector<double> baseCoords, 
                                  std::vector<double> corner1Coords,
                                  std::vector<double> corner2Coords) {
        std::vector<double> newCoords(3, 0);

        newCoords[0] = baseCoords[0] / 2 + corner1Coords[0] / 4 + corner2Coords[0] / 4;
        newCoords[1] = baseCoords[1] / 2 + corner1Coords[1] / 4 + corner2Coords[1] / 4;
        newCoords[2] = baseCoords[2] / 2 + corner1Coords[2] / 4 + corner2Coords[2] / 4;

        return newCoords;
    }

void Mesh::createCellCenterNode() {
    for (unsigned long i = 0; i < cells.size(); i++) {
        Cell *cell = &cells[i];
        Node *node1 = &nodes[cell->cornerNodeIDs[0]];
        Node *node2 = &nodes[cell->cornerNodeIDs[1]];
        Node *node3 = &nodes[cell->cornerNodeIDs[2]];
        Node *node4 = &nodes[cell->cornerNodeIDs[3]];

        std::vector<double> centerCoords(3, 0);

        centerCoords[0] = (node1->coords[0] + node2->coords[0] + 
                           node3->coords[0] + node4->coords[0]) / 4;
        centerCoords[1] = (node1->coords[1] + node2->coords[1] + 
                           node3->coords[1] + node4->coords[1]) / 4;
        centerCoords[2] = (node1->coords[2] + node2->coords[2] + 
                           node3->coords[2] + node4->coords[2]) / 4;
        long centerNodeID = getNewNodeID();
        Node *centerNode = new Node(centerNodeID, centerCoords, false, true);
        nodes.push_back(*centerNode);
        cell->centerNodeID = centerNodeID;
    }
}

void Mesh::createFaceInnerNodes() {
    for (unsigned long i = 0; i < faces.size(); i++) {
        Face *face = &faces[i];
        Node *node1 = &nodes[face->cornerNodeIDs[0]];
        Node *node2 = &nodes[face->cornerNodeIDs[1]];
        Node *node3 = &nodes[face->cornerNodeIDs[2]];

        std::vector<double> node1Coords = node1->coords;
        std::vector<double> node2Coords = node2->coords;
        std::vector<double> node3Coords = node3->coords;
        bool boundNode = node1->boundNode && node2->boundNode && node3->boundNode;

        std::vector<double> innerNode1Coords = calcNewCoords(node1Coords,
                                                             node2Coords,
                                                             node3Coords);
        long innerNode1ID = getNewNodeID();
        Node *innerNode1 = new Node(innerNode1ID, innerNode1Coords, boundNode, false);
        nodes.push_back(*innerNode1);
        face->innerNodeIDs.push_back(innerNode1ID);

        std::vector<double> innerNode2Coords = calcNewCoords(node2Coords,
                                                             node1Coords,
                                                             node3Coords);
        long innerNode2ID = getNewNodeID();
        Node *innerNode2 = new Node(innerNode2ID, innerNode2Coords, boundNode, false);
        nodes.push_back(*innerNode2);
        face->innerNodeIDs.push_back(innerNode2ID);

        std::vector<double> innerNode3Coords = calcNewCoords(node3Coords,
                                                             node1Coords,
                                                             node2Coords);
        long innerNode3ID = getNewNodeID();
        Node *innerNode3 = new Node(innerNode3ID, innerNode3Coords, boundNode, false);
        nodes.push_back(*innerNode3);
        face->innerNodeIDs.push_back(innerNode3ID);
    }
}

void Mesh::fillCellFaceRelation() {
    std::map<long, std::set<long>> cellIDToNodeIDs;
    for (Cell cell : cells) {
        cellIDToNodeIDs[cell.ID] = std::set<long>(cell.cornerNodeIDs.begin(), 
                                                  cell.cornerNodeIDs.end());
    }
    for (unsigned long i = 0; i < faces.size(); i++) {
        Face *face = &faces[i];
        std::set<long> faceNodeIDs(face->cornerNodeIDs.begin(), face->cornerNodeIDs.end());
        for (auto it = cellIDToNodeIDs.cbegin(); it != cellIDToNodeIDs.cend(); ++it) {
            if (std::includes(it->second.begin(), it->second.end(), 
                              faceNodeIDs.begin(), faceNodeIDs.end())) {
                face->cellIDs.push_back(it->first);
                cells[it->first].faceIDs.push_back(face->ID);
            }
        }
    }
}

void Mesh::fillNodeNodeRelation() {
    for (unsigned long i = 0; i < cells.size(); i++) {
        Cell *cell = &cells[i];
        Node *node = &nodes[cell->centerNodeID];
        std::map<long, std::vector<double>> innerNodeIDToVector;
        for (unsigned long j = 0; j < cell->faceIDs.size(); j++) {
            Face *face = &faces[cell->faceIDs[j]];
            for (unsigned long k = 0; k < face->innerNodeIDs.size(); k++) {
                Node *innerNode = &nodes[face->innerNodeIDs[k]];
                std::vector<double> vector(3, 0);
                vector[0] = node->coords[0] - innerNode->coords[0];
                vector[1] = node->coords[1] - innerNode->coords[1];
                vector[2] = node->coords[2] - innerNode->coords[2];
                innerNodeIDToVector[innerNode->ID] = vector;
            }
        }
        for (auto i = innerNodeIDToVector.cbegin(); i != innerNodeIDToVector.cend(); ++i) {
            for (auto j = innerNodeIDToVector.cbegin(); j != innerNodeIDToVector.cend(); ++j) {
                if (i == j) {
                    continue;
                }

                std::vector<double> vectorProduct(3, 0);
                vectorProduct[0] = i->second[1] * j->second[2] - i->second[2] * j->second[1];
                vectorProduct[1] = i->second[2] * j->second[0] - i->second[0] * j->second[2];
                vectorProduct[2] = i->second[0] * j->second[1] - i->second[1] * j->second[0];
                if ((std::abs(vectorProduct[0]) <= std::numeric_limits<double>::min()) &&
                    (std::abs(vectorProduct[1]) <= std::numeric_limits<double>::min()) &&
                    (std::abs(vectorProduct[2]) <= std::numeric_limits<double>::min())) {
                    cell->nodeIDToOppositeNodeID[i->first] = j->first;
                }
            }
        }
        assert(cell->nodeIDToOppositeNodeID.size() == 12);
    }
}