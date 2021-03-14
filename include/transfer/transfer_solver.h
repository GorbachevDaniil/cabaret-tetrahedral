#ifndef TransferSolver_hpp
#define TransferSolver_hpp

#include "solver.h"

class TransferSolver : public Solver {
   private:
    double cfl;
    Mesh *mesh;

   public:
    TransferSolver(double cfl, Mesh *mesh) {
        this->cfl = cfl;
        this->mesh = mesh;
    };

    double calcTau();
    void processPhase1(double tau);
    void processPhase2(double tau);
    void processPhase3(double tau);
    void prepareNextStep();

    void processPhase2BoundFace(Face *face, double tau);
    void processPhase2InnerFace(Face *face, double tau);
};

#endif
