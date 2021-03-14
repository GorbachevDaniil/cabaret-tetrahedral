#ifndef TransferInitializer_hpp
#define TransferInitializer_hpp

#include "initializer.h"

class TransferInitializer : public Initializer {
   public:
    void initialize(Mesh &mesh);
};

#endif
