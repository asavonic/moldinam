#pragma once

namespace md {

class LennardJonesConstants 
{
    public:

    LennardJonesConstants() {
        sigma = sigma_pow_2 = sigma_pow_6 = sigma_pow_12 = 1;
        sigmaf = sigmaf_pow_2 = sigmaf_pow_6 = sigmaf_pow_12 = 1;
        eps = eps_pow_6 = eps_pow_12 = 1;
    }

    void set_sigma( double _sigma ) {
        sigmaf = sigma = _sigma;
        sigmaf_pow_2 = sigma_pow_2 = std::pow( sigma, 2 );
        sigmaf_pow_6 = sigma_pow_6 = std::pow( sigma, 6 );
        sigmaf_pow_12 = sigma_pow_12 = std::pow( sigma, 12 );
    }

    void set_eps( double _eps ) {
        epsf = eps = _eps;
        epsf_pow_6 = eps_pow_6 = std::pow( eps, 6 );
        epsf_pow_12 = eps_pow_12 = std::pow( eps, 12 );
    }

    template<typename T = double>
    T get_sigma() const { return sigma; }

    template<typename T = double>
    T get_eps() const { return eps; }

    template<typename T = double>
    T get_sigma_pow_2() const { return sigma_pow_2; }

    template<typename T = double>
    T get_sigma_pow_6() const { return sigma_pow_6; }

    template<typename T = double>
    T get_sigma_pow_12() const { return sigma_pow_12; }

private:
    double sigma;
    double eps;

    float sigmaf;
    float epsf;

    // precomputed pow( sigma, N ) used due to performance reasons
    double sigma_pow_2;

    double sigma_pow_6;
    double eps_pow_6;

    double sigma_pow_12;
    double eps_pow_12;

    float sigmaf_pow_2;

    float sigmaf_pow_6;
    float epsf_pow_6;

    float sigmaf_pow_12;
    float epsf_pow_12;
};

template<>
inline float LennardJonesConstants::get_sigma<float>() const { return sigmaf; }
template<>
inline float LennardJonesConstants::get_sigma_pow_2<float>() const { return sigmaf_pow_2; }
template<>
inline float LennardJonesConstants::get_sigma_pow_6<float>() const { return sigmaf_pow_6; }
template<>
inline float LennardJonesConstants::get_sigma_pow_12<float>() const { return sigmaf_pow_12; }

template<>
inline float LennardJonesConstants::get_eps<float>() const { return epsf; }


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

} // namespace md
