#include "TransferSolver.hpp"

#include <armadillo>
#include <cassert>
#include <iomanip>
#include <limits>

#include <Eigen/Dense>

double TransferSolver::calcTau() {
    double tau = std::numeric_limits<double>::max();
    for (unsigned long i = 0; i < mesh->cells.size(); i++) {
        Cell *cell = &mesh->cells[i];
        for (auto faceID : cell->faceIDs) {
            for (auto nodeID : mesh->faces[faceID].innerNodeIDs) {
                // TODO calculate it once
                Eigen::Vector3d transfer = mesh->nodes[cell->centerNodeID].coords - 
                                           mesh->nodes[nodeID].coords;
                double h = 2 * transfer.norm();
                transfer.normalize();
                double lambda = mesh->v[cell->centerNodeID][0].dot(transfer);
                double nodeTau = h / std::abs(lambda);
                if (tau > nodeTau) {
                    tau = nodeTau;
                }
            }
        }
    }
    assert(tau != std::numeric_limits<double>::max());
    return cfl * tau;
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

            double cn = c.dot(face->normal * cell->faceToNormalDir[faceID]);
            div += cn * phi * face->square;
        }
        div /= cell->volume;
        mesh->s1[cell->centerNodeID][0] = mesh->s0[cell->centerNodeID][0] - tau * div / 2;
    }
}

double monotize(double inv2, double inv0, double invCenter0, double invOpposite0, double invCenter1,
                double lambda, double tau, double dirDerivative) {
    double Q = (invCenter1 - invCenter0) / (tau / 2) + lambda * dirDerivative;
    double min = std::min(std::min(inv0, invOpposite0), invCenter0);
    double max = std::max(std::max(inv0, invOpposite0), invCenter0);

    min += tau * Q;
    max += tau * Q;

    inv2 = std::max(inv2, min);
    inv2 = std::min(inv2, max);

    return inv2;
}

void TransferSolver::processPhase2BoundFace(Face *face, double tau) {
    Cell *fromCell = &mesh->cells[face->cellIDs[0]];

    double dotProduct = mesh->v[fromCell->centerNodeID][0].dot(
        fromCell->faceToNormalDir[face->ID] * face->normal
    );
    if (dotProduct <= 0) {
        for (long nodeID : face->innerNodeIDs) {
            mesh->s2[nodeID][0] = 0;
        }
        return;
    }

    Eigen::Vector3d grad;
    grad << 0, 0, 0;
    for (long faceID : fromCell->faceIDs) {
        Face *gradFace = &mesh->faces[faceID];

        double phi = (mesh->s0[gradFace->innerNodeIDs[0]][0] +
                      mesh->s0[gradFace->innerNodeIDs[1]][0] +
                      mesh->s0[gradFace->innerNodeIDs[2]][0]) / 3;
        grad += phi * gradFace->square * fromCell->faceToNormalDir[faceID] * gradFace->normal;
    }
    grad /= fromCell->volume;

    long centerNodeID = fromCell->centerNodeID;
    double lambda = mesh->v[centerNodeID][0].dot(
        fromCell->faceToNormalDir[face->ID] * face->normal
    );
    for (long nodeID : face->innerNodeIDs) {
        long oppositeNodeID = fromCell->nodeIDToOppositeNodeID[nodeID];

        double inv0 = mesh->s0[nodeID][0];
        double invCenter0 = mesh->s0[centerNodeID][0];
        double invCenter1 = mesh->s1[centerNodeID][0];
        double invOpposite0 = mesh->s0[oppositeNodeID][0];
        // double inv2 = 2 * invCenter1 - invOpposite0;
        double inv2 = invCenter1;

        // Eigen::Vector3d transfer = mesh->nodes[oppositeNodeID].coords - 
        //                            mesh->nodes[nodeID].coords;
        // double h = transfer.norm();
        // double dirDerivative = (invOpposite0 - inv0) / h;

        // inv2 = monotize(inv2, inv0, invCenter0, invOpposite0, invCenter1,
        //                 lambda, tau, dirDerivative);

        Eigen::Vector3d transfer = mesh->nodes[oppositeNodeID].coords - 
                                   mesh->nodes[nodeID].coords;
        transfer.normalize();

        inv2 = monotize(inv2, inv0, invCenter0, invOpposite0, invCenter1,
                        lambda, tau, grad.dot(fromCell->faceToNormalDir[face->ID] * face->normal));
        mesh->s2[nodeID][0] = inv2;
    }

    // Cell *fromCell = &mesh->cells[face->cellIDs[0]];
    // for (long nodeID : face->innerNodeIDs) {
    //     mesh->s2[nodeID][0] = mesh->s1[fromCell->centerNodeID][0];
    // }
}

void TransferSolver::processPhase2InnerFace(Face *face, double tau) {
    Cell *cell1 = &mesh->cells[face->cellIDs[0]];
    Cell *cell2 = &mesh->cells[face->cellIDs[1]];

    Eigen::Vector3d averageU = (mesh->v[cell1->centerNodeID][0] + 
                                mesh->v[cell2->centerNodeID][0]) / 2;
    double dotProduct = averageU.dot(face->normal * cell1->faceToNormalDir[face->ID]);
    
    Cell *fromCell;
    if (dotProduct > 0) {
        fromCell = cell1;
    } else {
        fromCell = cell2;
    }

    Eigen::Vector3d grad;
    grad << 0, 0, 0;
    for (long faceID : fromCell->faceIDs) {
        Face *gradFace = &mesh->faces[faceID];

        double phi = (mesh->s0[gradFace->innerNodeIDs[0]][0] +
                      mesh->s0[gradFace->innerNodeIDs[1]][0] +
                      mesh->s0[gradFace->innerNodeIDs[2]][0]) / 3;
        grad += phi * gradFace->square * fromCell->faceToNormalDir[faceID] * gradFace->normal;
    }
    grad /= fromCell->volume;

    long centerNodeID = fromCell->centerNodeID;
    double lambda = mesh->v[centerNodeID][0].dot(
        fromCell->faceToNormalDir[face->ID] * face->normal
    );
    for (long nodeID : face->innerNodeIDs) {
        long oppositeNodeID = fromCell->nodeIDToOppositeNodeID[nodeID];

        double inv0 = mesh->s0[nodeID][0];
        double invCenter0 = mesh->s0[centerNodeID][0];
        double invCenter1 = mesh->s1[centerNodeID][0];
        double invOpposite0 = mesh->s0[oppositeNodeID][0];
        // double inv2 = 2 * invCenter1 - invOpposite0;
        double inv2 = invCenter1;

        // Eigen::Vector3d transfer = mesh->nodes[oppositeNodeID].coords - 
        //                            mesh->nodes[nodeID].coords;
        // double h = transfer.norm();
        // double dirDerivative = (invOpposite0 - inv0) / h;

        // inv2 = monotize(inv2, inv0, invCenter0, invOpposite0, invCenter1,
        //                 lambda, tau, dirDerivative);

        Eigen::Vector3d transfer = mesh->nodes[oppositeNodeID].coords - 
                                   mesh->nodes[nodeID].coords;
        transfer.normalize();

        inv2 = monotize(inv2, inv0, invCenter0, invOpposite0, invCenter1,
                        lambda, tau, grad.dot(fromCell->faceToNormalDir[face->ID] * face->normal));
        mesh->s2[nodeID][0] = inv2;
    }

    // Cell *cell1 = &mesh->cells[face->cellIDs[0]];
    // Cell *cell2 = &mesh->cells[face->cellIDs[1]];

    // Eigen::Vector3d averageU = (mesh->v[cell1->centerNodeID][0] + 
    //                             mesh->v[cell2->centerNodeID][0]) / 2;
    // double dotProduct = averageU.dot(face->normal * cell1->faceToNormalDir[face->ID]);
    
    // Cell *fromCell;
    // if (dotProduct > 0) {
    //     fromCell = cell1;
    // } else {
    //     fromCell = cell2;
    // }

    // for (long nodeID : face->innerNodeIDs) {
    //     mesh->s2[nodeID][0] = mesh->s1[fromCell->centerNodeID][0];
    // }
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

            double cn = c.dot(face->normal * cell->faceToNormalDir[faceID]);
            div += cn * phi * face->square;
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
