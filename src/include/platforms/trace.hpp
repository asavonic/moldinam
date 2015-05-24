#pragma once

#include <functional>
#include <sstream>
#include <platforms/platform.hpp>

class TraceError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

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

    void attach(ParticleSystem& par_sys)
    {
        using namespace std::placeholders;
        ParticleSystem::IterationCb cb = std::bind(&TraceCollector::onInteration, this, _1);
        par_sys.registerOnIterationCb(cb);

    }

    void onInteration(ParticleSystem* pSys)
    {
        m_os << "onIteration" << std::endl;
    }

private:
    std::string m_filename;
    std::ofstream m_ofs;
    std::ostream& m_os;
};
