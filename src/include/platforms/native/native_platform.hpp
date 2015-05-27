#ifndef __NATIVE_PLATFORM_HPP
#define __NATIVE_PLATFORM_HPP


#include <platforms/platform.hpp>
#include <utils/config/particle_system_config.hpp>
#include <utils/config/lennard_jones_config.hpp>
#include <platforms/native/types.hpp>

#include <md_types.h> // legacy support

#include <istream>

using namespace md;

class NativeParticleSystem : public ParticleSystem {
public:
    NativeParticleSystem();

    explicit NativeParticleSystem(ParticleSystemConfig conf);

    virtual void loadParticles(float3vec&& pos, float3vec&& pos_prev, float3vec&& vel,
                               float3vec&& accel);

    virtual void loadParticles(std::istream& is, size_t num);
    virtual void loadParticles(std::istream& is);
    virtual void storeParticles(std::ostream& os);

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
    inline void singleLennardJonesInteraction(const float3& target_pos, const float3& other_pos,
                                              float3& target_accel);
    

    // Both particles changed at the same time
    // faster than single interaction, but requires second particle
    // to be avaliable for read and change
    inline void doubleLennardJonesInteraction(const float3& first_pos, const float3& second_pos,
                                              float3& first_accel, float3& second_accel);
    

    inline void computeLennardJonesForcePotential(double r, const LennardJonesConstants& constants,
                                                  double& force, double& potential);

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
