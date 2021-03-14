#ifndef AbstractInitializer_hpp
#define AbstractInitializer_hpp

#include "core/mesh.h"

class Initializer {
   public:
    virtual void initialize(Mesh &mesh) = 0;
};

#endif
