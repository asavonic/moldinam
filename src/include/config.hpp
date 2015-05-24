#ifndef __CONFIG_HPP
#define __CONFIG_HPP

#include <string>
#include <map>
#include <iostream>
#include <limits>
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

class ConfigError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class IConfigEntry {
public:
    IConfigEntry()
    {
    }

    IConfigEntry(std::string name) : m_name(name)
    {
    }

    virtual ~IConfigEntry() = 0;
    virtual void readValueFromStream(std::istream& is) = 0;
    virtual std::string& name() { return m_name; }
protected:
    std::string m_name;
};
inline IConfigEntry::~IConfigEntry() {}

template<typename T>
class ConfigEntry : public IConfigEntry {
public:
    ConfigEntry() {}
    ConfigEntry(T val, std::string name) : IConfigEntry(name),
                                            m_value(val)
    {
    }

    virtual ~ConfigEntry() {}

    ConfigEntry<T>& operator=(const T value) {
        m_value = value;
        return *this;
    }

    ConfigEntry<T>& operator=(const ConfigEntry<T> &rhs) {
        if (this == &rhs)
            return *this;

        IConfigEntry::operator=(rhs);

        m_value = rhs.m_value;

        return *this;
    }

    operator T() const { return m_value; }
    virtual T& value() { return m_value; }
    virtual const T& value() const { return m_value; }

    virtual void readValueFromStream(std::istream& is)
    {
        is >> m_value;
    }
private:
    T m_value;
};

class IConfig {
public:
    virtual void loadDefault() = 0;
    virtual void loadFromStream(std::istream& is);
    virtual std::string name() { return m_config_name; }

protected:
    virtual void onLoad()
    {
    }

    std::map<std::string, IConfigEntry*> m_strEntryMap;
    std::string m_config_name;
};

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

        m_strEntryMap[periodic.name()] = &periodic;
        m_strEntryMap[use_cutoff.name()] = &use_cutoff;
        m_strEntryMap[area_size.name()] = &area_size;
        m_strEntryMap[dt.name()] = &dt;
    }

    // made all config variables public to avoid function number explosion
    ConfigEntry<bool> periodic;
    ConfigEntry<bool> use_cutoff;
    ConfigEntry<md::float3> area_size;
    ConfigEntry<float> dt;
};

// TODO: implement constants for different types
class LennardJonesConfig : public IConfig {
public:
    LennardJonesConfig()
    {
        m_config_name = "LennardJonesConfig";
        loadDefault();
    }

    virtual void loadDefault()
    {
        m_sigma = ConfigEntry<float>(0.1, "sigma");
        m_eps = ConfigEntry<float>(0.001, "eps");

        m_strEntryMap[m_sigma.name()] = &m_sigma;
        m_strEntryMap[m_eps.name()] = &m_eps;

        onLoad();
    }

    virtual void onLoad()
    {
        temp_lj_constants.set_sigma(m_sigma);
        temp_lj_constants.set_eps(m_eps);
    }

    LennardJonesConstants getConstants()
    {
        return temp_lj_constants;
    }

private:
    ConfigEntry<float> m_sigma;
    ConfigEntry<float> m_eps;
    LennardJonesConstants temp_lj_constants;
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

    
} // namespace md

#endif /* __CONFIG_HPP */
