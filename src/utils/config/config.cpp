#include <utils/config/config.hpp>

template<>
void ConfigEntry<std::string>::readValueFromStream(std::istream& is)
{
    std::string line;
    std::getline(is, line);

    size_t open_quote = line.find('"');
    size_t close_quote = line.find('"', open_quote + 1);

    if (open_quote != std::string::npos && close_quote != std::string::npos) {
        m_value = line.substr(open_quote + 1, close_quote - 1);
    } else {
        throw ConfigError("Unable to find quoted string in :" + line);
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
