#ifndef __NATIVE_PLATFORM_HPP
#define __NATIVE_PLATFORM_HPP


#include <platforms/platform.hpp>
#include <utils/config/particle_system_config.hpp>
#include <utils/config/lennard_jones_config.hpp>
#include <platforms/native/types.hpp>
#include <utils/stream.hpp>

#include <md_types.h> // legacy support

#include <istream>

using namespace md;

class NativeParticleSystem : public ParticleSystem {
public:
    NativeParticleSystem();

    explicit NativeParticleSystem(ParticleSystemConfig conf);

    virtual void loadParticles(float3vec&& pos, float3vec&& pos_prev, float3vec&& vel,
                               float3vec&& accel);

    virtual void loadParticles(ParticleIStreamPtr is, size_t num);
    virtual void loadParticles(ParticleIStreamPtr is);
    virtual void storeParticles(ParticleOStreamPtr os);

    virtual void applyPeriodicConditions();
    virtual void applyVerletIntegration();
    virtual void applyEulerIntegration();
    virtual void applyLennardJonesInteraction();

    virtual void iterate(size_t iterations);
    

    const float3vec& pos() const { return m_pos; }
    const float3vec& pos_prev() const { return m_pos_prev; }
    const float3vec& vel() const { return m_vel; }
    const float3vec& accel() const { return m_accel; }

    float3vec& pos() { return m_pos; }
    float3vec& pos_prev() { return m_pos_prev; }
    float3vec& vel() { return m_vel; }
    float3vec& accel() { return m_accel; }

protected:
    void periodicLennardJonesInteraction();

    // Changes only target particle, other particle is not affected
    // Used when we have only read access to other particle,
    // e.g. when using distributed memory
    void singleLennardJonesInteraction(const float3& target_pos, const float3& other_pos,
                                       float3& target_accel,
                                       const LennardJonesConstants& lj_constants);
    

    // Both particles changed at the same time
    // faster than single interaction, but requires second particle
    // to be avaliable for read and change
    inline void doubleLennardJonesInteraction(const float3& first_pos, const float3& second_pos,
                                              float3& first_accel, float3& second_accel,
                                              const LennardJonesConstants& lj_constants);
    

    // rsqr is a perf hack to avoid sqrt() calls
    inline void computeLennardJonesForcePotential(float rsqr, const LennardJonesConstants& constants,
                                                  float& force, float& potential);

protected:
    float3vec m_pos;
    float3vec m_pos_prev;
    float3vec m_vel;
    float3vec m_accel;

    LennardJonesConfig m_lj_config;
};

namespace md {
namespace legacy {
    NativeParticleSystem convertToNativeSystem(const std::vector<Molecule>&, ParticleSystemConfig conf);
} // namespace legacy
} // namespace md

#endif /* __NATIVE_PLATFORM_HPP */
