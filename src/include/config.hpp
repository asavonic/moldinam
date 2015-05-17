#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <stdexcept>


#include <platforms/native/types.hpp>

namespace md {

class LennardJonesConstants 
{
    public:

    LennardJonesConstants() {
        sigma = sigma_pow_6 = sigma_pow_12 = 1;
        eps = eps_pow_6 = eps_pow_12 = 1;
    }

    void set_sigma( double _sigma ) {
        sigma = _sigma;
        sigma_pow_6 = std::pow( sigma, 6 );
        sigma_pow_12 = std::pow( sigma, 12 );
    }

    void set_eps( double _eps ) {
        eps = _eps;
        eps_pow_6 = std::pow( eps, 6 );
        eps_pow_12 = std::pow( eps, 12 );
    }

    template<typename T = double>
    T get_sigma() const { return sigma; }

    template<typename T = double>
    T get_eps() const { return eps; }

    template<typename T = double>
    T get_sigma_pow_6() const { return sigma_pow_6; }

    template<typename T = double>
    T get_sigma_pow_12() const { return sigma_pow_12; }

    private:

    double sigma;
    double eps;

    // precomputed pow( sigma, N ) used due to performance reasons
    double sigma_pow_6;
    double eps_pow_6;

    double sigma_pow_12;
    double eps_pow_12;
};

// TODO tests missing
class LennardJonesConfig {
public:
    LennardJonesConfig()
    {
    }

    LennardJonesConfig( std::string )
    {
    }

    void loadConfig(std::string config_path)
    {
        m_conf_filename = config_path;
    }

    LennardJonesConstants getConstants()
    {
        return temp_lj_constants;
    }

    // TODO: implement constants for different types
    // LennardJonesConstants get_constants( Molecule_Type type ) { return type_constants_map[ type ]; }

private:
    std::string m_conf_filename;
    LennardJonesConstants temp_lj_constants;
    // TODO: implement constants for different types
    // std::map<Molecule_Type, LennardJonesConstants> type_constants_map;
};

class ParticleSystemConfig {

    template<typename T>
    class ConfigEntry {
    public:
        ConfigEntry() {}
        ConfigEntry(T val, std::string name)
        {
            value_name_pair = std::make_pair(val, name);
        }

        operator T() const
        {
            return value_name_pair.first;
        }

        ConfigEntry<T>& operator=(const T value) {
            value_name_pair.first = value;
            return *this;
        }

        ConfigEntry<T>& operator=(const ConfigEntry<T> &rhs) {
            if (this == &rhs)
                return *this;

            value_name_pair = rhs.value_name_pair;

            return *this;
        }

        std::string& name() { return value_name_pair.second; }
        T& value() { return value_name_pair.first; }
    private:
        std::pair<T, std::string> value_name_pair;
    };

public:
    ParticleSystemConfig()
    {
        loadDefault();
    }

    explicit ParticleSystemConfig(std::string filename)
    {
        loadDefault();
        loadFromFile(filename);
    }

    void loadDefault()
    {
        periodic = ConfigEntry<bool>(false, "use_periodic");
        use_cutoff = ConfigEntry<bool>(false, "use_cutoff");
        area_size = ConfigEntry<md::float3>(md::float3(0), "area_size");
        dt = ConfigEntry<float>(0.000005, "dt");
    }

    void loadFromFile(std::string filename)
    {
        m_conf_filename = filename;
    }

    // made all config variables public to avoid function number explosion
    ConfigEntry<bool> periodic;
    ConfigEntry<bool> use_cutoff;
    ConfigEntry<md::float3> area_size;
    ConfigEntry<float> dt;

protected:
    std::string m_conf_filename;
};

class ConfigManager {
public:
    static ConfigManager& Instance()
    {
        static ConfigManager self;
        return self;
    }

    ConfigManager()
    {
    }

    ParticleSystemConfig getParticleSystemConfig() { return m_part_system_config; }
    LennardJonesConfig getLennardJonesConfig() { return m_lennard_jones_config; }

private:
    ParticleSystemConfig m_part_system_config;
    LennardJonesConfig m_lennard_jones_config;
};

    
} // namespace md

#endif /* __CONFIG_HPP */
