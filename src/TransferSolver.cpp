#include "TransferSolver.hpp"

#include <armadillo>
#include <cassert>
#include <iomanip>
#include <limits>

#include <Eigen/Dense>

double TransferSolver::calcTau() {
    return cfl;
}

void TransferSolver::processPhase1(double tau) {
    for (unsigned long cellID = 0; cellID < mesh->cells.size(); cellID++) {
        Cell *cell = &mesh->cells[cellID];
        double div = 0;

        for (long faceID : cell->faceIDs) {
            Face *face = &mesh->faces[faceID];

            double phi = (mesh->s0[face->innerNodeIDs[0]][0] +
                          mesh->s0[face->innerNodeIDs[1]][0] +
                          mesh->s0[face->innerNodeIDs[2]][0]) / 3;
            
            Eigen::Vector3d c = (mesh->v[face->innerNodeIDs[0]][0] +
                                 mesh->v[face->innerNodeIDs[1]][0] +
                                 mesh->v[face->innerNodeIDs[2]][0]) / 3;

            double cn = cell->faceToNormalDir[faceID] * c.dot(face->normal);
            div += cn * phi / face->square;
        }
        div /= cell->volume;
        mesh->s1[cell->centerNodeID][0] = mesh->s0[cell->centerNodeID][0] - tau * div / 2;
    }
}

void TransferSolver::processPhase2BoundFace(Face *face, double tau) {
    Cell *fromCell = &mesh->cells[face->cellIDs[0]];

    double dotProduct = mesh->v[fromCell->centerNodeID][0].dot(face->normal);
    if (dotProduct * fromCell->faceToNormalDir[face->ID] <= 0) {
        for (long nodeID : face->innerNodeIDs) {
            mesh->s2[nodeID][0] = 0;
        }
        return;
    }

    long centerNodeID = fromCell->centerNodeID;
    for (long nodeID : face->innerNodeIDs) {
        long oppositeNodeID = fromCell->nodeIDToOppositeNodeID[nodeID];

        double newS = 2 * mesh->s1[centerNodeID][0] - mesh->s0[oppositeNodeID][0];
        // TODO monotize invatiant
        mesh->s2[nodeID][0] = newS;
    }
}

void TransferSolver::processPhase2InnerFace(Face *face, double tau) {
    Cell *cell1 = &mesh->cells[face->cellIDs[0]];
    Cell *cell2 = &mesh->cells[face->cellIDs[1]];

    Eigen::Vector3d averageU = (mesh->v[cell1->centerNodeID][0] + 
                                mesh->v[cell2->centerNodeID][0]) / 2;
    double dotProduct = averageU.dot(face->normal);
    
    Cell *fromCell;
    if (dotProduct * cell1->faceToNormalDir[face->ID] > 0) {
        fromCell = cell1;
    } else {
        fromCell = cell2;
    }

    long centerNodeID = fromCell->centerNodeID;
    for (long nodeID : face->innerNodeIDs) {
        long oppositeNodeID = fromCell->nodeIDToOppositeNodeID[nodeID];

        double newS = 2 * mesh->s1[centerNodeID][0] - mesh->s0[oppositeNodeID][0];
        // TODO monotize invatiant
        mesh->s2[nodeID][0] = newS;
    }
}

void TransferSolver::processPhase2(double tau) {
    for (unsigned long faceID = 0; faceID < mesh->cells.size(); faceID++) {
        Face *face = &mesh->faces[faceID];

        if (face->boundFace) {
            processPhase2BoundFace(face, tau);
        } else {
            processPhase2InnerFace(face, tau);
        }
    }
}

void TransferSolver::processPhase3(double tau) {
    for (unsigned long cellID = 0; cellID < mesh->cells.size(); cellID++) {
        Cell *cell = &mesh->cells[cellID];
        double div = 0;

        for (long faceID : cell->faceIDs) {
            Face *face = &mesh->faces[faceID];

            double phi = (mesh->s2[face->innerNodeIDs[0]][0] +
                          mesh->s2[face->innerNodeIDs[1]][0] +
                          mesh->s2[face->innerNodeIDs[2]][0]) / 3;
            
            Eigen::Vector3d c = (mesh->v[face->innerNodeIDs[0]][0] +
                                 mesh->v[face->innerNodeIDs[1]][0] +
                                 mesh->v[face->innerNodeIDs[2]][0]) / 3;

            double cn = cell->faceToNormalDir[faceID] * c.dot(face->normal);
            div += cn * phi / face->square;
        }
        div /= cell->volume;
        mesh->s2[cell->centerNodeID][0] = mesh->s1[cell->centerNodeID][0] - tau * div / 2;
    }
}

void TransferSolver::prepareNextStep() {
    for (unsigned long i = 0; i < mesh->nodes.size(); i++) {
        mesh->s0[i][0] = mesh->s2[i][0];
    }
}
