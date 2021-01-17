#include "Mesh.hpp"
#include "TetGenParser.hpp"
#include "TransferInitializer.hpp"
#include "TransferSolver.hpp"
#include "TransferOutput.hpp"

#include <chrono>
#include <iostream>
#include <libconfig.h++>

#include <Eigen/Dense>

int main() {
    Mesh mesh = Mesh();

    TetGenParser parser = TetGenParser();
    parser.LoadNodes(&mesh, "mesh/cube/cube.1.node");
    std::cout << "Read nodes file" << std::endl;
    parser.LoadFaces(&mesh, "mesh/cube/cube.1.face");
    std::cout << "Read faces file" << std::endl;
    parser.LoadCells(&mesh, "mesh/cube/cube.1.ele");
    std::cout << "Read cells file" << std::endl;

    mesh.createCellCenterNode();
    std::cout << "Created cell center nodes" << std::endl;
    mesh.createFaceInnerNodes();
    std::cout << "Created face inner nodes" << std::endl;

    mesh.fillCellFaceRelation();
    std::cout << "Filled cell<->face relations" << std::endl;
    mesh.fillNodeNodeRelation();
    std::cout << "Filled node<->node relations" << std::endl;

    mesh.calculateCellVolume();
    std::cout << "Calculated cells volume" << std::endl;
    mesh.calculateFaceSquare();
    std::cout << "Calculated faces square" << std::endl;
    mesh.calculateFaceNormal();
    std::cout << "Calculated faces normal" << std::endl;

    TransferInitializer initializer = TransferInitializer();
    initializer.initialize(mesh);
    std::cout << "Initialized data on mesh" << std::endl;

    TransferOutput output = TransferOutput(1);
    output.writeCSV(&mesh, 0, 0);

    TransferSolver solver = TransferSolver(0.3, &mesh);
    double tau = solver.calcTau();
    std::cout << tau << std::endl;

    for (int i = 0; i < 10000; i++) {
        solver.processPhase1(tau);
        solver.processPhase2(tau);
        solver.processPhase3(tau);
        solver.prepareNextStep();
        if (i % 10 == 0) {
            output.writeCSV(&mesh, tau, i + 1);
        }
    }

    return 0;
}
