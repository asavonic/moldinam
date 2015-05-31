#include <platforms/tbb/tbb_platform.hpp>


TBBParticleSystem::TBBParticleSystem()
{
}

TBBParticleSystem::TBBParticleSystem(ParticleSystemConfig conf) : NativeParticleSystem(conf)
{
}

void TBBParticleSystem::applyLennardJonesInteraction()
{
    tbb::parallel_for(tbb::blocked_range<size_t>(0,m_pos.size()),
        [&](const tbb::blocked_range<size_t>& r) {
            for (size_t i = r.begin(), end = r.end(); i != end; i++) {
                for (int j = 0; j < i; j++) {
                    singleLennardJonesInteraction(m_pos[i], m_pos[j], m_accel[i]);
                }

                for (int j = i + 1; j < (int)m_pos.size(); j++) {
                    singleLennardJonesInteraction(m_pos[i], m_pos[j], m_accel[i]);
                }
            }
        }
    );
}
