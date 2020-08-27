#include "TransferInitializer.hpp"

#include <cmath>
#include <iostream>

void TransferInitializer::initialize(Mesh &mesh) {
    unsigned long nodesSize = mesh.nodes.size();

    mesh.s0 = std::vector<std::vector<double>>(nodesSize);
    mesh.s1 = std::vector<std::vector<double>>(nodesSize);
    mesh.s2 = std::vector<std::vector<double>>(nodesSize);
    mesh.v = std::vector<std::vector<Eigen::Vector3d>>(nodesSize);

    for (unsigned long i = 0; i < nodesSize; i++) {
        mesh.s0[i] = std::vector<double>(1);
        mesh.s1[i] = std::vector<double>(1);
        mesh.s2[i] = std::vector<double>(1);
        mesh.v[i] = std::vector<Eigen::Vector3d>(1);

        mesh.s0[i][0] = 0;
        mesh.s1[i][0] = 0;
        mesh.s2[i][0] = 0;

        Node *node = &mesh.nodes[i];
        double x = node->coords[0];
        double y = node->coords[1];
        double z = node->coords[2];

        if (x > 0.5) {
            mesh.s0[i][0] = 1;
        } else {
            mesh.s0[i][0] = 0;
        }

        mesh.v[i][0][0] = 1;
        mesh.v[i][0][1] = 0;
        mesh.v[i][0][2] = 0;
    }
}
