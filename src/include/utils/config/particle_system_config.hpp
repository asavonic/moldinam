#pragma once


#include <utils/config/config.hpp>
#include <platforms/native/types.hpp>

using namespace md;

class ParticleSystemConfig : public IConfig {
public:
    ParticleSystemConfig()
    {
        m_config_name = "ParticleSystemConfig";
        loadDefault();
    }

    virtual void loadDefault()
    {
        periodic = ConfigEntry<bool>(false, "periodic");
        use_cutoff = ConfigEntry<bool>(false, "use_cutoff");
        area_size = ConfigEntry<md::float3>(md::float3(0), "area_size");
        dt = ConfigEntry<float>(0.000005, "dt");
        particles_num = ConfigEntry<size_t>(0, "particles_num");
        init_file = ConfigEntry<std::string>("", "init_file");

        m_strEntryMap[periodic.name()] = &periodic;
        m_strEntryMap[use_cutoff.name()] = &use_cutoff;
        m_strEntryMap[area_size.name()] = &area_size;
        m_strEntryMap[dt.name()] = &dt;
        m_strEntryMap[particles_num.name()] = &particles_num;
        m_strEntryMap[init_file.name()] = &init_file;
    }

    // made all config variables public to avoid function number explosion
    ConfigEntry<bool> periodic;
    ConfigEntry<bool> use_cutoff;
    ConfigEntry<md::float3> area_size;
    ConfigEntry<float> dt;
    ConfigEntry<size_t> particles_num;
    ConfigEntry<std::string> init_file;
};
