#ifndef TransferOutput_hpp
#define TransferOutput_hpp

#include "output.h"

class TransferOutput : public Output {
   public:
    TransferOutput(int writePeriod) { this->writePeriod = writePeriod; };

    void writeCSV(Mesh *mesh, double time, int step);
};

#endif
