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

    // + partial spec for std::string in config.cpp
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

#endif /* __CONFIG_HPP */
