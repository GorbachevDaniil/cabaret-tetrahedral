#ifndef Parser_hpp
#define Parser_hpp

#include "Mesh.hpp"

class TetGenParser {
   public:
    int LoadNodes(Mesh *mesh, std::string filename);
    int LoadFaces(Mesh *mesh, std::string filename);
    int LoadCells(Mesh *mesh, std::string filename);
};

#endif
