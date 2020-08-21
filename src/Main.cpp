#include "Mesh.hpp"
#include "TetGenParser.hpp"
#include "TransferInitializer.hpp"

#include <chrono>
#include <iostream>
#include <libconfig.h++>

int main() {
    Mesh mesh = Mesh();

    TetGenParser parser = TetGenParser();
    parser.LoadNodes(&mesh, "mesh/cube/cube.1.node");
    parser.LoadFaces(&mesh, "mesh/cube/cube.1.face");
    parser.LoadCells(&mesh, "mesh/cube/cube.1.ele");

    mesh.createCellCenterNode();
    mesh.createFaceInnerNodes();

    mesh.fillCellFaceRelation();
    mesh.fillNodeNodeRelation();

    mesh.allocateScalars();
    mesh.allocateVectors();

    TransferInitializer initializer = TransferInitializer();
    initializer.initialize(mesh);

    return 0;
}
