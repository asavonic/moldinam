#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <string>
#include <config.hpp>
#include <platforms/native/types.hpp>

class ParticleSystem {
public:
    ParticleSystem()
    {
    }

    explicit ParticleSystem(ParticleSystemConfig conf)
    {
    }

    virtual void applyVerletIntegration() = 0;
    virtual void applyEulerIntegration() = 0;
};

#endif // __PLATFORM_H
