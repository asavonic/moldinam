#include <utils/trace.hpp>

using namespace md;

void TraceCollector::attach(ParticleSystem& par_sys)
{
    using namespace std::placeholders;
    ParticleSystem::IterationCb cb = std::bind(&TraceCollector::onInteration, this, _1, _2);
    par_sys.registerOnIterationCb(cb);
}

void TraceCollector::onInteration(ParticleSystem* pSys, size_t iteration)
{
    if (!m_trace_conf.enabled) {
        return;
    }

    if ((iteration - m_last_iteration) < m_trace_conf.iterations_threshold) {
        return;
    }

    pSys->storeParticles(m_os);

    m_last_iteration = iteration;
}
