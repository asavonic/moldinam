#ifndef __PLATFORM_H
#define __PLATFORM_H

#include <string>
#include <vector>
#include <functional>

#include <utils/config/particle_system_config.hpp>
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

    virtual void loadParticles(std::istream& is, size_t num) = 0;
    virtual void storeParticles(std::ostream& os) = 0;

    const ParticleSystemConfig& config() const { return m_config; }

    typedef std::function<void(ParticleSystem*)> IterationCb;
    virtual void registerOnIterationCb(IterationCb cb)
    {
        m_on_iter_cb.push_back(cb);
    }

    virtual void invokeOnIteration()
    {
        for(auto& cb : m_on_iter_cb) {
            cb(this);
        }
    }

protected:
    ParticleSystemConfig m_config;

    std::vector<IterationCb> m_on_iter_cb;
};

#endif // __PLATFORM_H
