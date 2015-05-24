#include <utils/config/config_manager.hpp>
    
void ConfigManager::loadFromFile(std::string filename)
{
    std::ifstream ifs(filename); 
    if (ifs.fail()) {
        throw std::runtime_error("Unable to open config: " + filename);
    }

    for (std::string line; std::getline(ifs, line); ) {
        size_t open_bracket = line.find('[');
        size_t close_bracket = line.find(']', open_bracket);

        if (open_bracket == std::string::npos ||
            close_bracket == std::string::npos)
        {
            throw ConfigError("Unexpected content: " + line);
        }

        std::string config_name = line.substr(open_bracket + 1, close_bracket - open_bracket - 1);
        if (m_strConfMap.count(config_name)) {
            m_strConfMap[config_name]->loadFromStream(ifs);
        } else {
            throw ConfigError("Unknown config: " + config_name);
        }
    }
}

