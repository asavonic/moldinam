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
        threshold = ConfigEntry<float>(0, "threshold");

        m_strEntryMap[filename.name()] = &filename;
        m_strEntryMap[threshold.name()] = &threshold;
    }

    ConfigEntry<std::string> filename;
    ConfigEntry<float> threshold;
};
