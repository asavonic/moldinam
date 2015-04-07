#ifndef __NATIVE_PLATFORM_HPP
#define __NATIVE_PLATFORM_HPP


#include <platforms/platform.hpp>
#include <config.hpp>
#include <platforms/native/types.hpp>

using namespace md;

class NativeParticleSystem : public ParticleSystem {
public:
    NativeParticleSystem()
    {
    }
        
    explicit NativeParticleSystem(ParticleSystemConfig conf) : ParticleSystem(conf)
    {
    }

    virtual void applyPeriodicConditions()
    {
        for (size_t i = 0; i < m_pos.size(); i++) {
            float3 shift = m_area_size * floor(m_pos[i] / m_area_size);
            m_pos[i] -= shift;
            m_pos_prev[i] -= shift;
        }
    }

    virtual void applyVerletIntegration()
    {
        // TODO add config variable for verlet dt
        float dt = 0.00005;
        for (size_t i = 0; i < m_pos.size(); i++) {
            m_pos_prev[i] = 2.0f * m_pos[i] - m_pos_prev[i] + m_accel[i] * dt * dt;
        }

        // TODO add periodic config variable
        bool periodic = false;
        if (periodic) {
            applyPeriodicConditions();
        }

        std::swap(m_pos, m_pos_prev);
    }

    virtual void applyEulerIntegration()
    {
        // TODO add config variable
        float dt = 0.00005;
        for (size_t i = 0; i < m_pos.size(); i++) {
            m_pos[i] = m_pos[i] + m_vel[i] * dt;
        }

        bool periodic = false;
        if (periodic) {
            applyPeriodicConditions();
        }
    }


    virtual void applyLennardJonesInteraction()
    {
        bool periodic = false;
        if (periodic) {
            periodicLennardJonesInteraction();
        }

        for (int i = 0; i < m_pos.size() - 1; i++) {
            for (int j = i + 1; j < m_pos.size(); j++) {
                doubleLennardJonesInteraction(m_pos[i], m_pos[j], m_accel[i], m_accel[j]);
            }
        }

    }
protected:
    void periodicLennardJonesInteraction()
    {
        
    }

    // Changes only target particle, other particle is not affected
    // Used when we have only read access to other particle,
    // e.g. when using distributed memory
    inline void singleLennardJonesInteraction(const float3& target_pos, const float3& other_pos, float3& target_accel)
    {
        float r = distance(target_pos, other_pos);

        auto lj_constants = m_lj_config.getConstants();

        // TODO add use_cutoff config
        bool use_cutoff = false;
        if (use_cutoff && r > 2.5 * lj_constants.get_sigma()) {
            return;
        }

        double force_scalar = 0;
        double potential = 0;
        computeLennardJonesForcePotential(r, lj_constants, force_scalar, potential);

        float3 force_direction = (target_pos - other_pos) / r;
        float3 force_vec = force_direction * (float)force_scalar;

        target_accel += force_vec;
    }

    // Both particles changed at the same time
    // faster than single interaction, but requires second particle
    // to be avaliable for read and change
    inline void doubleLennardJonesInteraction(const float3& first_pos, const float3& second_pos,
                                              float3& first_accel, float3& second_accel)
    {
        float r = distance(first_pos, second_pos);

        auto lj_constants = m_lj_config.getConstants();

        // TODO add use_cutoff config
        bool use_cutoff = false;
        if (use_cutoff && r > 2.5 * lj_constants.get_sigma()) {
            return;
        }

        double force_scalar = 0;
        double potential = 0;
        computeLennardJonesForcePotential(r, lj_constants, force_scalar, potential);

        float3 force_direction = (first_pos - second_pos) / r;
        float3 force_vec = force_direction * (float)force_scalar;

        first_accel += force_vec;
        second_accel -= force_vec;
    }

    inline void computeLennardJonesForcePotential(double r, const LennardJonesConstants& constants,
                                             double& force, double& potential)
    {
        double ri = 1 / r;
        double ri3 = ri * ri * ri;
        double ri6 = ri3 * ri3;

        force = 48 * constants.get_eps<float>() * (constants.get_sigma_pow_12<float>() * ri6 - constants.get_sigma_pow_6<float>() / 2) * ri6 * ri * ri;
        potential = 4 * constants.get_eps<float>() * ri6 * (ri6 * constants.get_sigma_pow_12<float>() - constants.get_sigma_pow_6<float>());
    }


protected:
    float3vec m_pos;
    float3vec m_pos_prev;
    float3vec m_vel;
    float3vec m_accel;

    float3 m_area_size;

    LennardJonesConfig m_lj_config;
};

#endif /* __NATIVE_PLATFORM_HPP */
