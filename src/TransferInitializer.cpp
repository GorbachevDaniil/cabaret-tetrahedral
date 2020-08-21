#include "TransferInitializer.hpp"

#include <cmath>
#include <iostream>

void TransferInitializer::initialize(Mesh &mesh) {
    for (unsigned long i = 0; i < mesh.nodes.size(); i++) {

        mesh.s0[i] = std::vector<double>(1);
        mesh.s1[i] = std::vector<double>(1);
        mesh.s2[i] = std::vector<double>(1);

        mesh.v0[i] = std::vector<std::vector<double>>(1);
        mesh.v1[i] = std::vector<std::vector<double>>(1);
        mesh.v2[i] = std::vector<std::vector<double>>(1);

        mesh.s0[i][0] = 0;
        mesh.s1[i][0] = 0;
        mesh.s2[i][0] = 0;
        mesh.v0[i][0] = std::vector<double>(3, 0);
        mesh.v1[i][0] = std::vector<double>(3, 0);
        mesh.v2[i][0] = std::vector<double>(3, 0);
    }

    for (unsigned long i = 0; i < mesh.nodes.size(); i++) {
        Node *node = &mesh.nodes[i];
        double x = node->coords[0];
        double y = node->coords[1];
        double z = node->coords[2];

        if (x > 0) {
            mesh.s0[i][0] = 1;
        } else {
            mesh.s0[i][0] = 0;
        }
    }
}
