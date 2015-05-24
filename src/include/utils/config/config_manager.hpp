#pragma once

#include <utils/config/particle_system_config.hpp>
#include <utils/config/lennard_jones_config.hpp>
#include <utils/config/trace_config.hpp>

class ConfigManager {
public:
    static ConfigManager& Instance()
    {
        static ConfigManager self;
        return self;
    }

    ConfigManager()
    {
        m_strConfMap[m_part_system_config.name()] = &m_part_system_config;
        m_strConfMap[m_lennard_jones_config.name()] = &m_lennard_jones_config;
    }

    ParticleSystemConfig getParticleSystemConfig() { return m_part_system_config; }
    LennardJonesConfig getLennardJonesConfig() { return m_lennard_jones_config; }

    void loadFromFile(std::string filename);

private:
    std::map<std::string, IConfig*> m_strConfMap;
    ParticleSystemConfig m_part_system_config;
    LennardJonesConfig m_lennard_jones_config;
};
