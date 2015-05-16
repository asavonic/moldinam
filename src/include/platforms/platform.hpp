#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <string>
#include <config.hpp>
#include <platforms/native/types.hpp>

using namespace md;

class ParticleSystem {
public:
    ParticleSystem()
    {
    }

    explicit ParticleSystem(ParticleSystemConfig conf) : m_config(conf)
    {
    }

    virtual void applyVerletIntegration() = 0;
    virtual void applyEulerIntegration() = 0;

    const ParticleSystemConfig& config() const { return m_config; }
protected:
    ParticleSystemConfig m_config;
};

#endif // __PLATFORM_H
