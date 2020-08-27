#include "Mesh.hpp"

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cassert>

Eigen::Vector3d calcNewCoords(Eigen::Vector3d baseCoords, 
                              Eigen::Vector3d corner1Coords,
                              Eigen::Vector3d corner2Coords) {
        Eigen::Vector3d newCoords;

        newCoords = baseCoords / 2 + corner1Coords / 4 + corner2Coords / 4;

        return newCoords;
    }

void Mesh::createCellCenterNode() {
    for (unsigned long i = 0; i < cells.size(); i++) {
        Cell *cell = &cells[i];
        Node *node1 = &nodes[cell->cornerNodeIDs[0]];
        Node *node2 = &nodes[cell->cornerNodeIDs[1]];
        Node *node3 = &nodes[cell->cornerNodeIDs[2]];
        Node *node4 = &nodes[cell->cornerNodeIDs[3]];

        Eigen::Vector3d centerCoords;
        centerCoords = (node1->coords + node2->coords + 
                        node3->coords + node4->coords) / 4;
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

        Eigen::Vector3d node1Coords = node1->coords;
        Eigen::Vector3d node2Coords = node2->coords;
        Eigen::Vector3d node3Coords = node3->coords;
        bool boundNode = node1->boundNode && node2->boundNode && node3->boundNode;

        Eigen::Vector3d innerNode1Coords = calcNewCoords(node1Coords,
                                                         node2Coords,
                                                         node3Coords);
        long innerNode1ID = getNewNodeID();
        Node *innerNode1 = new Node(innerNode1ID, innerNode1Coords, boundNode, false);
        nodes.push_back(*innerNode1);
        face->innerNodeIDs.push_back(innerNode1ID);

        Eigen::Vector3d innerNode2Coords = calcNewCoords(node2Coords,
                                                         node1Coords,
                                                         node3Coords);
        long innerNode2ID = getNewNodeID();
        Node *innerNode2 = new Node(innerNode2ID, innerNode2Coords, boundNode, false);
        nodes.push_back(*innerNode2);
        face->innerNodeIDs.push_back(innerNode2ID);

        Eigen::Vector3d innerNode3Coords = calcNewCoords(node3Coords,
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
        std::map<long, Eigen::Vector3d> innerNodeIDToVector;
        for (unsigned long j = 0; j < cell->faceIDs.size(); j++) {
            Face *face = &faces[cell->faceIDs[j]];
            for (unsigned long k = 0; k < face->innerNodeIDs.size(); k++) {
                Node *innerNode = &nodes[face->innerNodeIDs[k]];
                innerNodeIDToVector[innerNode->ID] = node->coords - innerNode->coords;
            }
        }
        for (auto i = innerNodeIDToVector.cbegin(); i != innerNodeIDToVector.cend(); ++i) {
            for (auto j = innerNodeIDToVector.cbegin(); j != innerNodeIDToVector.cend(); ++j) {
                if (i == j) {
                    continue;
                }

                Eigen::Vector3d vectorProduct;
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

void Mesh::calculateCellVolume() {
    for (unsigned long i = 0; i < cells.size(); i++) {
        Cell *cell = &cells[i];
        Node *node1 = &nodes[cell->cornerNodeIDs[0]];
        Node *node2 = &nodes[cell->cornerNodeIDs[1]];
        Node *node3 = &nodes[cell->cornerNodeIDs[2]];
        Node *node4 = &nodes[cell->cornerNodeIDs[3]];

        Eigen::Vector3d vector12 = node2->coords - node1->coords;
        Eigen::Vector3d vector13 = node3->coords - node1->coords;
        Eigen::Vector3d vector14 = node4->coords - node1->coords;

        double det = vector12[0] * vector13[1] * vector14[2] +
                     vector12[2] * vector13[0] * vector14[1] +
                     vector12[2] * vector13[1] * vector14[0] -
                     vector12[0] * vector13[2] * vector14[1] -
                     vector12[1] * vector13[0] * vector14[3];
        cell->volume = det / 6;
    }
}

void Mesh::calculateFaceSquare() {
    for (unsigned long i = 0; i < faces.size(); i++) {
        Face *face = &faces[i];
        Node *node1 = &nodes[face->cornerNodeIDs[0]];
        Node *node2 = &nodes[face->cornerNodeIDs[1]];
        Node *node3 = &nodes[face->cornerNodeIDs[2]];

        Eigen::Vector3d vector12 = node2->coords - node1->coords;
        Eigen::Vector3d vector13 = node3->coords - node1->coords;

        Eigen::Vector3d vectorProduct;
        vectorProduct[0] = vector12[1] * vector13[2] - vector12[2] * vector13[1];
        vectorProduct[1] = vector12[2] * vector13[0] - vector12[0] * vector13[2];
        vectorProduct[2] = vector12[0] * vector13[1] - vector12[1] * vector13[0];

        face->square = (vectorProduct[0] *  vectorProduct[0] + 
                        vectorProduct[1] *  vectorProduct[1] +
                        vectorProduct[2] *  vectorProduct[2]) / 2;
    }
}

void Mesh::calculateFaceNormal() {
    for (unsigned long i = 0; i < faces.size(); i++) {
        Face *face = &faces[i];
        Node *node1 = &nodes[face->cornerNodeIDs[0]];
        Node *node2 = &nodes[face->cornerNodeIDs[1]];
        Node *node3 = &nodes[face->cornerNodeIDs[2]];

        double b1 = node2->coords[0] - node1->coords[0];
        double b2 = node2->coords[1] - node1->coords[1];
        double b3 = node2->coords[2] - node1->coords[2];
        double c1 = node3->coords[0] - node1->coords[0];
        double c2 = node3->coords[1] - node1->coords[1];
        double c3 = node3->coords[2] - node1->coords[2];

        // A * x + B * y + C * z + D = 0
        double A = b2 * c3 - b3 * c2;
        double B = b3 * c1 - b1 * c3;
        double C = b1 * c2 - b2 * c1;
        double D = -(A+B+C);

        double normalCoef = std::sqrt(A * A + B * B + C * C);
        face->normal[0] = A / normalCoef;
        face->normal[1] = B / normalCoef;
        face->normal[2] = C / normalCoef;

        Eigen::Vector3d proxy(3, 0);
        proxy[0] = node1->coords[0] + A;
        proxy[1] = node1->coords[1] + B;
        proxy[2] = node1->coords[2] + C;
        bool isProxyAbove = (A * proxy[0] + B * proxy[1] + C * proxy[2] + D) > 0;

        for (long cellID : face->cellIDs) {
            Cell *cell = &cells[cellID];
            Eigen::Vector3d coords = nodes[cell->centerNodeID].coords;

            bool isCenterAbove = (A * coords[0] + B * coords[1] + C * coords[2] + D) > 0;
            if (isProxyAbove && isCenterAbove) {
                cell->faceToNormalDir[i] = 1;
            } else {
                cell->faceToNormalDir[i] = -1;
            }
        }
    }
}