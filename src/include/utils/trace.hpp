#pragma once

#include <functional>
#include <sstream>
#include <platforms/platform.hpp>
#include <utils/config/config.hpp>
#include <utils/config/config_manager.hpp>
#include <utils/config/trace_config.hpp>

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

        if (filename != "") {
            m_ofs.open(filename);
            if (!m_ofs.good()) {
                throw TraceError("Unable to open trace file" + filename);
            }
        }

        if (m_trace_conf.enabled.value()) {
            if (filename != "") {
                m_os = &m_ofs;
            } else {
                // switch to stdout if no file specified, but trace enabled
                m_os = &std::cout;
            }
        }
    }

    void attach(ParticleSystem& par_sys);
    void onInteration(ParticleSystem* pSys, size_t iteration);

private:
    std::ostream* m_os;

    size_t m_last_iteration;
    TraceConfig m_trace_conf;

    std::ofstream m_ofs;
};
