#ifndef TransferOutput_hpp
#define TransferOutput_hpp

#include "AbstractOutput.hpp"

class TransferOutput : public AbstractOutput {
   public:
    TransferOutput(int writePeriod) { this->writePeriod = writePeriod; };

    void writeCSV(Mesh *mesh, double time, int step);
};

#endif
