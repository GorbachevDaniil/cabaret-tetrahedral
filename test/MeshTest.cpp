#include "catch.hpp"

#include "Node.hpp"
#include "Face.hpp"
#include "Cell.hpp"
#include "Mesh.hpp"

#include <vector>
#include <algorithm>
#include <string>
#include <map>

TEST_CASE("createCellCenterNode", "Positive") {
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;

    double x2 = 1;
    double y2 = 0;
    double z2 = 0;

    double x3 = 0;
    double y3 = 1;
    double z3 = 0;

    double x4 = 0;
    double y4 = 0;
    double z4 = 1;

    Node node1(0, x1, y1, z1, false, false);
    Node node2(1, x2, y2, z2, false, false);
    Node node3(2, x3, y3, z3, false, false);
    Node node4(3, x4, y4, z4, false, false);

    Cell cell(0, 0, 1, 2, 3);

    Mesh mesh;
    mesh.nodes.push_back(node1);
    mesh.nodes.push_back(node2);
    mesh.nodes.push_back(node3);
    mesh.nodes.push_back(node4);
    mesh.cells.push_back(cell);

    mesh.createCellCenterNode();

    CHECK(mesh.nodes.size() == 5);
    CHECK(mesh.nodes[0].coords.size() == 3);
    CHECK(mesh.nodes[1].coords.size() == 3);
    CHECK(mesh.nodes[2].coords.size() == 3);
    CHECK(mesh.nodes[3].coords.size() == 3);
    CHECK(mesh.nodes[4].coords.size() == 3);

    CHECK(mesh.nodes[4].ID == 4);
    CHECK(mesh.nodes[4].coords[0] == Approx(0.25));
    CHECK(mesh.nodes[4].coords[1] == Approx(0.25));
    CHECK(mesh.nodes[4].coords[2] == Approx(0.25));

    CHECK(mesh.cells[0].centerNodeID == 4);
}

TEST_CASE("createFaceInnerNodes", "Positive") {
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;

    double x2 = 4;
    double y2 = 0;
    double z2 = 0;

    double x3 = 0;
    double y3 = 4;
    double z3 = 0;

    Node node1(0, x1, y1, z1, false, false);
    Node node2(1, x2, y2, z2, false, false);
    Node node3(2, x3, y3, z3, false, false);

    Face face(0, 0, 1, 2, false);

    Mesh mesh;
    mesh.nodes.push_back(node1);
    mesh.nodes.push_back(node2);
    mesh.nodes.push_back(node3);
    mesh.faces.push_back(face);
    
    mesh.createFaceInnerNodes();

    CHECK(mesh.nodes.size() == 6);
    CHECK(mesh.nodes[0].coords.size() == 3);
    CHECK(mesh.nodes[1].coords.size() == 3);
    CHECK(mesh.nodes[2].coords.size() == 3);
    CHECK(mesh.nodes[3].coords.size() == 3);
    CHECK(mesh.nodes[4].coords.size() == 3);
    CHECK(mesh.nodes[5].coords.size() == 3);

    CHECK(mesh.nodes[3].ID == 3);
    CHECK(mesh.nodes[3].coords[0] == Approx(1));
    CHECK(mesh.nodes[3].coords[1] == Approx(1));
    CHECK(mesh.nodes[3].coords[2] == Approx(0));

    CHECK(mesh.nodes[4].ID == 4);
    CHECK(mesh.nodes[4].coords[0] == Approx(2));
    CHECK(mesh.nodes[4].coords[1] == Approx(1));
    CHECK(mesh.nodes[4].coords[2] == Approx(0));

    CHECK(mesh.nodes[5].ID == 5);
    CHECK(mesh.nodes[5].coords[0] == Approx(1));
    CHECK(mesh.nodes[5].coords[1] == Approx(2));
    CHECK(mesh.nodes[5].coords[2] == Approx(0));

    CHECK(mesh.faces[0].innerNodeIDs[0] == 3);
    CHECK(mesh.faces[0].innerNodeIDs[1] == 4);
    CHECK(mesh.faces[0].innerNodeIDs[2] == 5);
}

TEST_CASE("fillCellFaceRelation", "Positive") {
    Face face1(0, 0, 1, 2, false);
    Face face2(1, 0, 1, 3, false);
    Face face3(2, 1, 3, 2, false);
    Face face4(3, 0, 3, 2, false);
    Face face5(4, 2, 4, 1, false);
    Face face6(5, 2, 4, 3, false);
    Face face7(6, 1, 4, 3, false);

    Cell cell1(0, 0, 1, 2, 3);
    Cell cell2(1, 4, 1, 2, 3);

    Mesh mesh;
    mesh.faces.push_back(face1);
    mesh.faces.push_back(face2);
    mesh.faces.push_back(face3);
    mesh.faces.push_back(face4);
    mesh.faces.push_back(face5);
    mesh.faces.push_back(face6);
    mesh.faces.push_back(face7);
    mesh.cells.push_back(cell1);
    mesh.cells.push_back(cell2);

    mesh.fillCellFaceRelation();

    std::vector<long> cell1FaceIDs = mesh.cells[0].faceIDs;
    std::sort(cell1FaceIDs.begin(), cell1FaceIDs.begin());
    CHECK(cell1FaceIDs.size() == 4);
    CHECK(cell1FaceIDs[0] == 0);
    CHECK(cell1FaceIDs[1] == 1);
    CHECK(cell1FaceIDs[2] == 2);
    CHECK(cell1FaceIDs[3] == 3);

    std::vector<long> cell2FaceIDs = mesh.cells[1].faceIDs;
    std::sort(cell2FaceIDs.begin(), cell2FaceIDs.begin());
    CHECK(cell2FaceIDs.size() == 4);
    CHECK(cell2FaceIDs[0] == 2);
    CHECK(cell2FaceIDs[1] == 4);
    CHECK(cell2FaceIDs[2] == 5);
    CHECK(cell2FaceIDs[3] == 6);

    std::vector<long> face1CellIDs = mesh.faces[0].cellIDs;
    CHECK(face1CellIDs.size() == 1);
    CHECK(face1CellIDs[0] == 0);

    std::vector<long> face2CellIDs = mesh.faces[1].cellIDs;
    CHECK(face2CellIDs.size() == 1);
    CHECK(face2CellIDs[0] == 0);

    std::vector<long> face3CellIDs = mesh.faces[2].cellIDs;
    CHECK(face3CellIDs.size() == 2);
    CHECK(face3CellIDs[0] == 0);
    CHECK(face3CellIDs[1] == 1);

    std::vector<long> face4CellIDs = mesh.faces[3].cellIDs;
    CHECK(face4CellIDs.size() == 1);
    CHECK(face4CellIDs[0] == 0);

    std::vector<long> face5CellIDs = mesh.faces[4].cellIDs;
    CHECK(face5CellIDs.size() == 1);
    CHECK(face5CellIDs[0] == 1);

    std::vector<long> face6CellIDs = mesh.faces[5].cellIDs;
    CHECK(face6CellIDs.size() == 1);
    CHECK(face6CellIDs[0] == 1);

    std::vector<long> face7CellIDs = mesh.faces[6].cellIDs;
    CHECK(face7CellIDs.size() == 1);
    CHECK(face7CellIDs[0] == 1);
}

TEST_CASE("fillNodeNodeRelation", "Positive") {
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;

    double x2 = 1;
    double y2 = 0;
    double z2 = 0;

    double x3 = 0;
    double y3 = 1;
    double z3 = 0;

    double x4 = 0;
    double y4 = 0;
    double z4 = 1;

    Node node1(0, x1, y1, z1, false, false);
    Node node2(1, x2, y2, z2, false, false);
    Node node3(2, x3, y3, z3, false, false);
    Node node4(3, x4, y4, z4, false, false);

    Face face1(0, 0, 1, 2, false);
    Face face2(1, 0, 1, 3, false);
    Face face3(2, 1, 3, 2, false);
    Face face4(3, 0, 3, 2, false);

    Cell cell(0, 0, 1, 2, 3);

    Mesh mesh;
    mesh.nodes.push_back(node1);
    mesh.nodes.push_back(node2);
    mesh.nodes.push_back(node3);
    mesh.nodes.push_back(node4);
    mesh.faces.push_back(face1);
    mesh.faces.push_back(face2);
    mesh.faces.push_back(face3);
    mesh.faces.push_back(face4);
    mesh.cells.push_back(cell);

    mesh.createCellCenterNode();
    mesh.createFaceInnerNodes();
    mesh.fillCellFaceRelation();
    mesh.fillNodeNodeRelation();

    CHECK(mesh.cells[0].nodeIDToOppositeNodeID.size() == 12);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[5] == 12);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[6] == 15);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[7] == 10);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[8] == 13);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[9] == 16);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[10] == 7);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[11] == 14);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[12] == 5);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[13] == 8);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[14] == 11);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[15] == 6);
    CHECK(mesh.cells[0].nodeIDToOppositeNodeID[16] == 9);
}

TEST_CASE("XYFaceNoCells", "calculateFaceNormal") {
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;

    double x2 = 1;
    double y2 = 0;
    double z2 = 0;

    double x3 = 0;
    double y3 = 1;
    double z3 = 0;

    Node node1(0, x1, y1, z1, false, false);
    Node node2(1, x2, y2, z2, false, false);
    Node node3(2, x3, y3, z3, false, false);

    Face face(0, 0, 1, 2, false);

    Mesh mesh;
    mesh.nodes.push_back(node1);
    mesh.nodes.push_back(node2);
    mesh.nodes.push_back(node3);
    mesh.faces.push_back(face);

    mesh.calculateFaceNormal();

    CHECK(mesh.faces[0].normal[0] == 0);
    CHECK(mesh.faces[0].normal[1] == 0);
    CHECK(mesh.faces[0].normal[2] == 1);
}

TEST_CASE("XZFaceNoCells", "calculateFaceNormal") {
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;

    double x2 = 1;
    double y2 = 0;
    double z2 = 0;

    double x3 = 0;
    double y3 = 0;
    double z3 = 1;

    Node node1(0, x1, y1, z1, false, false);
    Node node2(1, x2, y2, z2, false, false);
    Node node3(2, x3, y3, z3, false, false);

    Face face(0, 0, 1, 2, false);

    Mesh mesh;
    mesh.nodes.push_back(node1);
    mesh.nodes.push_back(node2);
    mesh.nodes.push_back(node3);
    mesh.faces.push_back(face);

    mesh.calculateFaceNormal();

    CHECK(mesh.faces[0].normal[0] == 0);
    CHECK(mesh.faces[0].normal[1] == -1);
    CHECK(mesh.faces[0].normal[2] == 0);
}

TEST_CASE("YZFaceNoCells", "calculateFaceNormal") {
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;

    double x2 = 0;
    double y2 = 1;
    double z2 = 0;

    double x3 = 0;
    double y3 = 0;
    double z3 = 1;

    Node node1(0, x1, y1, z1, false, false);
    Node node2(1, x2, y2, z2, false, false);
    Node node3(2, x3, y3, z3, false, false);

    Face face(0, 0, 1, 2, false);

    Mesh mesh;
    mesh.nodes.push_back(node1);
    mesh.nodes.push_back(node2);
    mesh.nodes.push_back(node3);
    mesh.faces.push_back(face);

    mesh.calculateFaceNormal();

    CHECK(mesh.faces[0].normal[0] == 1);
    CHECK(mesh.faces[0].normal[1] == 0);
    CHECK(mesh.faces[0].normal[2] == 0);
}

TEST_CASE("FaceWithCells", "calculateFaceNormal") {
    double x1 = 0;
    double y1 = 0;
    double z1 = 0;

    double x2 = 1;
    double y2 = 0;
    double z2 = 0;

    double x3 = 0;
    double y3 = 1;
    double z3 = 0;

    double x4 = 0;
    double y4 = 0;
    double z4 = 1;

    double x5 = 0;
    double y5 = 0;
    double z5 = -1;

    Node node1(0, x1, y1, z1, false, false);
    Node node2(1, x2, y2, z2, false, false);
    Node node3(2, x3, y3, z3, false, false);
    Node node4(3, x4, y4, z4, false, false);
    Node node5(4, x5, y5, z5, false, false);

    Face face(0, 0, 1, 2, false);

    Cell cell1(0, 0, 1, 2, 3);
    Cell cell2(1, 0, 1, 2, 4);

    Mesh mesh;
    mesh.nodes.push_back(node1);
    mesh.nodes.push_back(node2);
    mesh.nodes.push_back(node3);
    mesh.nodes.push_back(node4);
    mesh.nodes.push_back(node5);
    mesh.faces.push_back(face);
    mesh.cells.push_back(cell1);
    mesh.cells.push_back(cell2);

    mesh.createCellCenterNode();
    mesh.fillCellFaceRelation();
    mesh.calculateFaceNormal();

    CHECK(mesh.faces[0].normal[0] == 0);
    CHECK(mesh.faces[0].normal[1] == 0);
    CHECK(mesh.faces[0].normal[2] == 1);
    CHECK(mesh.cells[0].faceToNormalDir[0] == -1);
    CHECK(mesh.cells[1].faceToNormalDir[0] == 1);
}