#include <platforms/native/native_platform.hpp>

NativeParticleSystem::NativeParticleSystem()
{
}

NativeParticleSystem::NativeParticleSystem(ParticleSystemConfig conf) : ParticleSystem(conf)
{
    size_t particles_num = m_config.particles_num;
    std::string init_file = m_config.init_file;
    if (init_file != "") {
        if (particles_num == 0) {
            throw std::runtime_error("particles_num is not defined in config file!");
        }

        ParticleIStreamPtr init = StreamFactory::Instance()->MakeInitIStream(m_config);
        loadParticles(init);
    }
}

void NativeParticleSystem::loadParticles(float3vec&& pos, float3vec&& pos_prev,
                                         float3vec&& vel,float3vec&& accel)
{
    m_pos = pos;
    m_pos_prev = pos_prev;
    m_vel = vel;
    m_accel = accel;
}

void NativeParticleSystem::loadParticles(ParticleIStreamPtr is, size_t num)
{
    m_pos.resize(num);
    m_pos_prev.resize(num);
    m_vel.resize(num);
    m_accel.resize(num);

    for (size_t i = 0; i < num; i++) {
        is->Read(m_pos[i], m_vel[i], m_accel[i]);
    }
}

void NativeParticleSystem::loadParticles(ParticleIStreamPtr is)
{
    loadParticles(is, m_config.particles_num);
}

void NativeParticleSystem::storeParticles(ParticleOStreamPtr os)
{
    for (size_t i = 0; i < m_pos.size(); i++) {
        os->Write(m_pos[i], m_vel[i], m_accel[i]);
    }
}

void NativeParticleSystem::applyPeriodicConditions()
{
    float3 area_size = m_config.area_size;
    for (size_t i = 0; i < m_pos.size(); i++) {
        float3 shift = area_size * floor(m_pos[i] / area_size);
        m_pos[i] -= shift;
        m_pos_prev[i] -= shift;
    }
}

void NativeParticleSystem::applyVerletIntegration()
{
    float dt = m_config.dt;
    for (size_t i = 0; i < m_pos.size(); i++) {
        m_pos_prev[i] = 2.0f * m_pos[i] - m_pos_prev[i] + m_accel[i] * dt * dt;
    }

    std::swap(m_pos, m_pos_prev);

    bool periodic = m_config.periodic;
    if (periodic) {
        applyPeriodicConditions();
    }
}

void NativeParticleSystem::applyEulerIntegration()
{
    float dt = m_config.dt;
    for (size_t i = 0; i < m_pos.size(); i++) {
        m_pos_prev[i] = m_pos[i] + m_vel[i] * dt + m_accel[i] * dt * dt;
    }

    std::swap(m_pos, m_pos_prev);

    bool periodic = false;
    if (periodic) {
        applyPeriodicConditions();
    }
}

void NativeParticleSystem::applyLennardJonesInteraction()
{
    bool periodic = false;
    if (periodic) {
        periodicLennardJonesInteraction();
    }

    #pragma omp parallel for 
    for (int i = 0; i < (int)m_pos.size(); i++) {
        for (int j = 0; j < i; j++) {
            singleLennardJonesInteraction(m_pos[i], m_pos[j], m_accel[i]);
        }

        for (int j = i + 1; j < (int)m_pos.size(); j++) {
            singleLennardJonesInteraction(m_pos[i], m_pos[j], m_accel[i]);
        }
    }

}

void NativeParticleSystem::periodicLennardJonesInteraction()
{

}

// Changes only target particle, other particle is not affected
// Used when we have only read access to other particle,
// e.g. when using distributed memory
void
NativeParticleSystem::singleLennardJonesInteraction(const float3& target_pos, const float3& other_pos,
                                                    float3& target_accel)
{
    float r_sqr = sqr_distance(target_pos, other_pos);
    auto lj_constants = m_lj_config.getConstants();

    bool use_cutoff = m_config.use_cutoff;
    if (use_cutoff && r_sqr > 2.5 * 2.5 * lj_constants.get_sigma_pow_2<float>()) {
        return;
    }

    float force_scalar = 0;
    float potential = 0;
    computeLennardJonesForcePotential(r_sqr, lj_constants, force_scalar, potential);

    float3 force_direction = (target_pos - other_pos) / sqrtf(r_sqr);
    float3 force_vec = force_direction * force_scalar;

    target_accel += force_vec;
}

// Both particles changed at the same time
// faster than single interaction, but requires second particle
// to be avaliable for read and change
inline void
NativeParticleSystem::doubleLennardJonesInteraction(const float3& first_pos, const float3& second_pos,
                                                    float3& first_accel, float3& second_accel)
{
    float r_sqr = sqr_distance(first_pos, second_pos);
    auto lj_constants = m_lj_config.getConstants();

    bool use_cutoff = m_config.use_cutoff;
    if (use_cutoff && r_sqr > 2.5 * 2.5 * lj_constants.get_sigma_pow_2<float>()) {
        return;
    }

    float force_scalar = 0;
    float potential = 0;
    computeLennardJonesForcePotential(r_sqr, lj_constants, force_scalar, potential);

    float3 force_direction = (first_pos - second_pos) / sqrtf(r_sqr);
    float3 force_vec = force_direction * force_scalar;

    first_accel += force_vec;
    second_accel -= force_vec;
}

inline void
NativeParticleSystem::computeLennardJonesForcePotential(float r_sqr, const LennardJonesConstants& constants,
                                                        float& force, float& potential)
{
    float ri_sqr = 1 / r_sqr;
    float ri6 = ri_sqr * ri_sqr * ri_sqr;
    float ri8 = ri8 * ri_sqr;

    force = 48 * constants.get_eps<float>() * ri8 *
        (constants.get_sigma_pow_12<float>() * ri6 - constants.get_sigma_pow_6<float>() / 2);

    potential = 4 * constants.get_eps<float>() * ri6 *
        (ri6 * constants.get_sigma_pow_12<float>() - constants.get_sigma_pow_6<float>());
}


NativeParticleSystem
md::legacy::convertToNativeSystem(const std::vector<Molecule>& legacy_mol_vec,
                                  ParticleSystemConfig conf)
{
    NativeParticleSystem native(conf);
    
    size_t num = legacy_mol_vec.size();

    float3vec pos(num);
    float3vec pos_prev(num);
    float3vec vel(num);
    float3vec accel(num);

    for (size_t i = 0; i < num; i++) {
        pos[i].x = legacy_mol_vec[i].pos.x;
        pos[i].y = legacy_mol_vec[i].pos.y;
        pos[i].z = legacy_mol_vec[i].pos.z;

        pos_prev[i].x = legacy_mol_vec[i].pos_prev.x;
        pos_prev[i].y = legacy_mol_vec[i].pos_prev.y;
        pos_prev[i].z = legacy_mol_vec[i].pos_prev.z;

        vel[i].x = legacy_mol_vec[i].speed.x;
        vel[i].y = legacy_mol_vec[i].speed.y;
        vel[i].z = legacy_mol_vec[i].speed.z;

        accel[i].x = legacy_mol_vec[i].accel.x;
        accel[i].y = legacy_mol_vec[i].accel.y;
        accel[i].z = legacy_mol_vec[i].accel.z;
    }

    native.loadParticles(std::move(pos), std::move(pos_prev),
                         std::move(vel), std::move(accel));

    return native;
}


void NativeParticleSystem::iterate(size_t iterations)
{
    bool periodic = m_config.periodic;

    applyEulerIntegration(); // to compute pos_prev

    for (size_t i = 0; i < iterations; ++i) {
        applyLennardJonesInteraction();
        applyVerletIntegration();

        if (periodic) {
            applyPeriodicConditions();
        }

        invokeOnIteration(i);
    }
}
