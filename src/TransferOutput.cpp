#include "TransferOutput.hpp"

#include <iostream>
#include <string>

#include <Eigen/Dense>

void TransferOutput::writeCSV(Mesh *mesh, double time, int step) {
    if (step % writePeriod != 0) {
        return;
    }

    FILE *output_f;

    std::string filename = "./bin/output/result." + std::to_string(step) + ".txt";
    output_f = std::fopen(filename.c_str(), "w");

    std::fprintf(output_f, "x,y,z,phi\n");

    Eigen::Vector3d coords;
    for (unsigned long i = 0; i < mesh->cells.size(); i++) {
        Cell *cell = &mesh->cells[i];
        coords = mesh->nodes[cell->centerNodeID].coords;
        std::fprintf(output_f, "%f,%f,%f,%f\n",
                     coords[0], coords[1], coords[2], mesh->s0[cell->centerNodeID][0]);
        // for (unsigned long faceID : cell->faceIDs) {
        //     Face *face = &mesh->faces[faceID];
        //     for (unsigned long nodeID : face->innerNodeIDs) {
        //         coords = mesh->nodes[nodeID].coords;
        //         std::fprintf(output_f, "%f,%f,%f,%f\n",
        //                      coords[0], coords[1], coords[2], mesh->s0[nodeID][0]);
        //     }
        // }
    }
    std::fclose(output_f);
}
