#include <platforms/native/native_platform.hpp>

NativeParticleSystem::NativeParticleSystem()
{
}

NativeParticleSystem::NativeParticleSystem(ParticleSystemConfig conf) : ParticleSystem(conf)
{
}

void NativeParticleSystem::loadParticles(float3vec&& pos, float3vec&& pos_prev,
                                         float3vec&& vel,float3vec&& accel)
{
    m_pos = pos;
    m_pos_prev = pos_prev;
    m_vel = vel;
    m_accel = accel;
}

void NativeParticleSystem::loadParticles(std::istream& is, size_t num)
{
    m_pos.resize(num);
    m_pos_prev.resize(num);
    m_vel.resize(num);
    m_accel.resize(num);

    for (size_t i = 0; i < num; i++) {
        is >> m_pos[i] >> m_vel[i] >> m_accel[i];
    }
}

void NativeParticleSystem::storeParticles(std::ostream& os)
{
    for (size_t i = 0; i < m_pos.size(); i++) {
        os << m_pos[i] << " " << m_vel[i] << " " << m_accel[i] << "\n";
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

    for (int i = 0; i < (int)m_pos.size() - 1; i++) {
        for (int j = i + 1; j < (int)m_pos.size(); j++) {
            doubleLennardJonesInteraction(m_pos[i], m_pos[j], m_accel[i], m_accel[j]);
        }
    }

}

void NativeParticleSystem::periodicLennardJonesInteraction()
{

}

// Changes only target particle, other particle is not affected
// Used when we have only read access to other particle,
// e.g. when using distributed memory
inline void
NativeParticleSystem::singleLennardJonesInteraction(const float3& target_pos, const float3& other_pos,
                                                    float3& target_accel)
{
    float r = distance(target_pos, other_pos);

    auto lj_constants = m_lj_config.getConstants();

    bool use_cutoff = m_config.use_cutoff;
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
inline void
NativeParticleSystem::doubleLennardJonesInteraction(const float3& first_pos, const float3& second_pos,
                                                    float3& first_accel, float3& second_accel)
{
    float r = distance(first_pos, second_pos);

    auto lj_constants = m_lj_config.getConstants();

    bool use_cutoff = m_config.use_cutoff;
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

inline void
NativeParticleSystem::computeLennardJonesForcePotential(double r, const LennardJonesConstants& constants,
                                                        double& force, double& potential)
{
    double ri = 1 / r;
    double ri3 = ri * ri * ri;
    double ri6 = ri3 * ri3;

    force = 48 * constants.get_eps<float>() * ri6 * ri * ri *
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
