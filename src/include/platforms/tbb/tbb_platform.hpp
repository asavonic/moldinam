#pragma once

#include <platforms/native/native_platform.hpp>
#include "tbb/tbb.h"

class TBBParticleSystem : public NativeParticleSystem {
public:
    TBBParticleSystem();
    explicit TBBParticleSystem(ParticleSystemConfig conf);
    virtual void applyLennardJonesInteraction();
};
