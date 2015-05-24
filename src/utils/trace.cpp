#include <platforms/trace.hpp>

using namespace md;

void TraceCollector::attach(ParticleSystem& par_sys)
{
    using namespace std::placeholders;
    ParticleSystem::IterationCb cb = std::bind(&TraceCollector::onInteration, this, _1);
    par_sys.registerOnIterationCb(cb);
}

void TraceCollector::onInteration(ParticleSystem* pSys)
{
    m_os << "onIteration" << std::endl;
}
