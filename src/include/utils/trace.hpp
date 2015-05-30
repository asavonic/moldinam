#pragma once

#include <functional>
#include <sstream>
#include <platforms/platform.hpp>
#include <utils/config/config.hpp>
#include <utils/config/config_manager.hpp>
#include <utils/config/trace_config.hpp>
#include <utils/stream.hpp>

using namespace md;

class TraceError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class TraceConfig;
class ParticleSystem;

class TraceCollector {
public:
    TraceCollector() : m_last_iteration(0), m_os(nullptr)
    {
        m_trace_conf = ConfigManager::Instance().getTraceConfig();

        std::string filename = m_trace_conf.filename.value();

        // TODO: open ofs from trace
    }

    void attach(ParticleSystem& par_sys);
    void onInteration(ParticleSystem* pSys, size_t iteration);

private:
    size_t m_last_iteration;
    TraceConfig m_trace_conf;

    ParticleOStreamPtr m_os;
};
