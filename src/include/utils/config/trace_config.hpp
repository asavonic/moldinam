#include <utils/config/config.hpp>

class TraceConfig : public IConfig {
public:
    TraceConfig()
    {
        m_config_name = "TraceConfig";
        loadDefault();
    }

    virtual void loadDefault()
    {
        filename = ConfigEntry<std::string>("", "filename");
        value_threshold = ConfigEntry<float>(0, "value_threshold");
        iterations_threshold = ConfigEntry<size_t>(0, "iterations_threshold");

        m_strEntryMap[filename.name()] = &filename;
        m_strEntryMap[value_threshold.name()] = &value_threshold;
        m_strEntryMap[iterations_threshold.name()] = &iterations_threshold;
    }

    ConfigEntry<std::string> filename;
    ConfigEntry<float> value_threshold;
    ConfigEntry<size_t> iterations_threshold;
};
