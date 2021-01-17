#ifndef AbstractOutput_hpp
#define AbstractOutput_hpp

#include "Mesh.hpp"

class AbstractOutput {
   protected:
    int writePeriod;
    bool writeConservative;
    bool writeFlux;

   public:
    virtual void writeCSV(Mesh *mesh, double time, int step) = 0;
};

#endif
