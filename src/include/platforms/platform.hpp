#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <string>

#include "md_helpers.h"

typedef double3 AreaSize;

class ParticleSystemConfig {
public:
    ParticleSystemConfig() : m_periodic(false)
    {
    }

    explicit ParticleSystemConfig(std::string filename) : m_conf_filename(filename)
    {
        LoadConfig();
    }

protected:
    std::string m_conf_filename;

    bool m_periodic;
    // TODO: LennardJonesConfig m_lj_config;
    AreaSize m_area_size;

protected:
    void LoadConfig()
    {
        // TODO: use m_filename to load config from file
    }
};

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
