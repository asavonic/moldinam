#include <config.hpp>

namespace md {
    
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

        std::string config_name = line.substr(open_bracket + 1, close_bracket - 1);
        if (m_strConfMap.count(config_name)) {
            m_strConfMap[config_name]->loadFromStream(ifs);
        } else {
            throw ConfigError("Unknown config: " + config_name);
        }
    }
}

void IConfig::loadFromStream(std::istream& is)
{
    loadDefault();
    std::string entry_name;
    bool read = true;
    while (read) {
        while (char c = is.get()) {
            if (!is.good()) {
                read = false;
                break;
            }

            // trim spaces at the beginning
            if (c == ' ' || c == '\n' || c == '\r') {
                continue;
            }

            // skip comment line
            if (c == '#') {
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            // next config header, finish parsing
            if (c == '[') {
                read = false;
            }

            is.unget();
            break;
        }

        if (!read) {
            break;
        }

        is >> entry_name;

        if (m_strEntryMap.count(entry_name)) {
            m_strEntryMap[entry_name]->readValueFromStream(is);
        } else {
            throw ConfigError("Unrecognized config entry: " + entry_name);
        }
    }

    onLoad();
}

} // namespace md
