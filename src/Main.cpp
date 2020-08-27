#include "Mesh.hpp"
#include "TetGenParser.hpp"
#include "TransferInitializer.hpp"
#include "TransferSolver.hpp"

#include <chrono>
#include <iostream>
#include <libconfig.h++>

#include <Eigen/Dense>

int main() {
    // Mesh mesh = Mesh();

    // TetGenParser parser = TetGenParser();
    // parser.LoadNodes(&mesh, "mesh/cube/cube.1.node");
    // parser.LoadFaces(&mesh, "mesh/cube/cube.1.face");
    // parser.LoadCells(&mesh, "mesh/cube/cube.1.ele");

    // mesh.createCellCenterNode();
    // mesh.createFaceInnerNodes();

    // mesh.fillCellFaceRelation();
    // mesh.fillNodeNodeRelation();

    // mesh.calculateCellVolume();
    // mesh.calculateFaceSquare();
    // mesh.calculateFaceNormal();

    // mesh.allocateScalars();
    // mesh.allocateVectors();

    // TransferInitializer initializer = TransferInitializer();
    // initializer.initialize(mesh);

    // TransferSolver solver = TransferSolver(0.3, &mesh);
    // solver.processPhase1(0.3);
    // solver.processPhase2(0.3);
    // solver.processPhase3(0.3);

    return 0;
}
