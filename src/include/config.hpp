#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <stdexcept>


#include <platforms/native/types.hpp>

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
public:
    ParticleSystemConfig() : m_periodic(false), m_area_size(md::float3(0,0,0)),
                             dt(0.000005)
    {
    }

    explicit ParticleSystemConfig(std::string filename)
    {
        loadConfig(filename);
    }

    void loadConfig(std::string filename)
    {
        m_conf_filename = filename;
    }

protected:
    std::string m_conf_filename;

    bool m_periodic;
    md::float3 m_area_size;
    float dt;
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

#endif /* __CONFIG_HPP */
