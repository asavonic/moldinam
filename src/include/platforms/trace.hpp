#pragma once

#include <functional>
#include <sstream>
#include <platforms/platform.hpp>
#include <utils/config/config.hpp>

using namespace md;

class TraceError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class TraceConfig;
class ParticleSystem;

class TraceCollector {
public:
    TraceCollector(std::string filename) :
        m_filename(filename),
        m_ofs(filename),
        m_os(m_ofs)
    {
        if (!m_ofs.good()) {
            throw TraceError("Unable to open trace file" + filename);
        }
    }

    TraceCollector(std::ostream& os) :
        m_os(os)
    {
    }

    void attach(ParticleSystem& par_sys);
    void onInteration(ParticleSystem* pSys);

private:
    std::string m_filename;
    std::ofstream m_ofs;
    std::ostream& m_os;
};
