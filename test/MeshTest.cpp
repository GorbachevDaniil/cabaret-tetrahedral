#include "gtest/gtest.h"

#include "Node.hpp"
#include "Face.hpp"
#include "Cell.hpp"
#include "Mesh.hpp"

#include <vector>
#include <algorithm>

TEST(createCellCenterNode, Positive) {
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

    EXPECT_EQ((unsigned long) 5, mesh.nodes.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[0].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[1].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[2].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[3].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[4].coords.size());

    EXPECT_EQ(4, mesh.nodes[4].ID);
    EXPECT_EQ(0.25, mesh.nodes[4].coords[0]);
    EXPECT_EQ(0.25, mesh.nodes[4].coords[1]);
    EXPECT_EQ(0.25, mesh.nodes[4].coords[2]);

    EXPECT_EQ(4, mesh.cells[0].centerNodeID);
}

TEST(createFaceInnerNodes, Positive) {
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

    EXPECT_EQ((unsigned long) 6, mesh.nodes.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[0].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[1].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[2].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[3].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[4].coords.size());
    EXPECT_EQ((unsigned long) 3, mesh.nodes[5].coords.size());

    EXPECT_EQ(3, mesh.nodes[3].ID);
    EXPECT_EQ(1, mesh.nodes[3].coords[0]);
    EXPECT_EQ(1, mesh.nodes[3].coords[1]);
    EXPECT_EQ(0, mesh.nodes[3].coords[2]);

    EXPECT_EQ(4, mesh.nodes[4].ID);
    EXPECT_EQ(2, mesh.nodes[4].coords[0]);
    EXPECT_EQ(1, mesh.nodes[4].coords[1]);
    EXPECT_EQ(0, mesh.nodes[4].coords[2]);

    EXPECT_EQ(5, mesh.nodes[5].ID);
    EXPECT_EQ(1, mesh.nodes[5].coords[0]);
    EXPECT_EQ(2, mesh.nodes[5].coords[1]);
    EXPECT_EQ(0, mesh.nodes[5].coords[2]);

    EXPECT_EQ(3, mesh.faces[0].innerNodeIDs[0]);
    EXPECT_EQ(4, mesh.faces[0].innerNodeIDs[1]);
    EXPECT_EQ(5, mesh.faces[0].innerNodeIDs[2]);
}

TEST(fillCellFaceRelation, Positive) {
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
    EXPECT_EQ((unsigned long) 4, cell1FaceIDs.size());
    EXPECT_EQ(0, cell1FaceIDs[0]);
    EXPECT_EQ(1, cell1FaceIDs[1]);
    EXPECT_EQ(2, cell1FaceIDs[2]);
    EXPECT_EQ(3, cell1FaceIDs[3]);

    std::vector<long> cell2FaceIDs = mesh.cells[1].faceIDs;
    std::sort(cell2FaceIDs.begin(), cell2FaceIDs.begin());
    EXPECT_EQ((unsigned long) 4, cell2FaceIDs.size());
    EXPECT_EQ(2, cell2FaceIDs[0]);
    EXPECT_EQ(4, cell2FaceIDs[1]);
    EXPECT_EQ(5, cell2FaceIDs[2]);
    EXPECT_EQ(6, cell2FaceIDs[3]);

    std::vector<long> face1CellIDs = mesh.faces[0].cellIDs;
    EXPECT_EQ((unsigned long) 1, face1CellIDs.size());
    EXPECT_EQ(0, face1CellIDs[0]);

    std::vector<long> face2CellIDs = mesh.faces[1].cellIDs;
    EXPECT_EQ((unsigned long) 1, face2CellIDs.size());
    EXPECT_EQ(0, face2CellIDs[0]);

    std::vector<long> face3CellIDs = mesh.faces[2].cellIDs;
    EXPECT_EQ((unsigned long) 2, face3CellIDs.size());
    EXPECT_EQ(0, face3CellIDs[0]);
    EXPECT_EQ(1, face3CellIDs[1]);

    std::vector<long> face4CellIDs = mesh.faces[3].cellIDs;
    EXPECT_EQ((unsigned long) 1, face4CellIDs.size());
    EXPECT_EQ(0, face4CellIDs[0]);

    std::vector<long> face5CellIDs = mesh.faces[4].cellIDs;
    EXPECT_EQ((unsigned long) 1, face5CellIDs.size());
    EXPECT_EQ(1, face5CellIDs[0]);

    std::vector<long> face6CellIDs = mesh.faces[5].cellIDs;
    EXPECT_EQ((unsigned long) 1, face6CellIDs.size());
    EXPECT_EQ(1, face6CellIDs[0]);

    std::vector<long> face7CellIDs = mesh.faces[6].cellIDs;
    EXPECT_EQ((unsigned long) 1, face7CellIDs.size());
    EXPECT_EQ(1, face7CellIDs[0]);
}

TEST(fillNodeNodeRelation, Positive) {
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

    EXPECT_EQ((unsigned long) 12, mesh.cells[0].nodeIDToOppositeNodeID.size());
    EXPECT_EQ(12, mesh.cells[0].nodeIDToOppositeNodeID[5]);
    EXPECT_EQ(15, mesh.cells[0].nodeIDToOppositeNodeID[6]);
    EXPECT_EQ(10, mesh.cells[0].nodeIDToOppositeNodeID[7]);
    EXPECT_EQ(13, mesh.cells[0].nodeIDToOppositeNodeID[8]);
    EXPECT_EQ(16, mesh.cells[0].nodeIDToOppositeNodeID[9]);
    EXPECT_EQ(7,  mesh.cells[0].nodeIDToOppositeNodeID[10]);
    EXPECT_EQ(14, mesh.cells[0].nodeIDToOppositeNodeID[11]);
    EXPECT_EQ(5,  mesh.cells[0].nodeIDToOppositeNodeID[12]);
    EXPECT_EQ(8,  mesh.cells[0].nodeIDToOppositeNodeID[13]);
    EXPECT_EQ(11, mesh.cells[0].nodeIDToOppositeNodeID[14]);
    EXPECT_EQ(6,  mesh.cells[0].nodeIDToOppositeNodeID[15]);
    EXPECT_EQ(9,  mesh.cells[0].nodeIDToOppositeNodeID[16]);
}