#ifndef AbstractSolver_hpp
#define AbstractSolver_hpp

#include "Mesh.hpp"

class AbstractSolver {
   public:
    virtual double calcTau() = 0;
    virtual void processPhase1(double tau) = 0;
    virtual void processPhase2(double tau) = 0;
    virtual void processPhase3(double tau) = 0;
    virtual void prepareNextStep() = 0;

    virtual void processPhase2BoundFace(Face *face, double tau) = 0;
    virtual void processPhase2InnerFace(Face *face, double tau) = 0;
};

#endif
