#include "gtest/gtest.h"

#include <stdexcept>

#include <platforms/native/native_platform.hpp>
#include <platforms/native/types.hpp>
#include <utils/config/config_manager.hpp>
#include "utils.hpp"

using namespace md;

TEST(config, lj_psys)
{
    ConfigManager& conf_man = ConfigManager::Instance();
    conf_man.loadFromFile("config_test.conf");

    LennardJonesConfig lj_conf = conf_man.getLennardJonesConfig();
    ParticleSystemConfig psys_conf = conf_man.getParticleSystemConfig();

    LennardJonesConstants lj_const = lj_conf.getConstants();

    ASSERT_EQ(true, psys_conf.periodic);
    ASSERT_EQ(float3(10, 11, 12), psys_conf.area_size.value());

    ASSERT_FLOAT_EQ(0.2, lj_const.get_sigma());
    ASSERT_FLOAT_EQ(0.001, lj_const.get_eps());
}

TEST(config, neg_not_exist)
{
    ConfigManager& conf_man = ConfigManager::Instance();
    ASSERT_THROW(conf_man.loadFromFile("foobar.conf"), std::runtime_error);
}

TEST(config, neg1)
{
    ConfigManager& conf_man = ConfigManager::Instance();
    ASSERT_THROW(conf_man.loadFromFile("config_test_neg1.conf"), ConfigError);
}

TEST(config, neg2)
{
    ConfigManager& conf_man = ConfigManager::Instance();
    ASSERT_THROW(conf_man.loadFromFile("config_test_neg2.conf"), ConfigError);
}

TEST(config, trace)
{
    ConfigManager& conf_man = ConfigManager::Instance();
    conf_man.loadFromFile("config_test_trace.conf");
    TraceConfig trace_config = conf_man.getTraceConfig();
    ASSERT_EQ("testfile.file", trace_config.filename.value());
    ASSERT_FLOAT_EQ(0.1, trace_config.threshold);
}

TEST(config, init_file_native)
{
    ConfigManager& conf_man = ConfigManager::Instance();
    conf_man.loadFromFile("config_test_init_file.conf");
    NativeParticleSystem native(conf_man.getParticleSystemConfig());

    std::stringstream ss;
    native.storeParticles(ss);

    std::ifstream ifs("config_test_init_file.data");

    size_t lines_num = 0;
    for(std::string native_data, ref; ifs.good() && ss.good(); ) {
        std::getline(ss, native_data);
        std::getline(ifs, ref);
        lines_num++;

        ASSERT_EQ(native_data, ref);
    }
    ASSERT_EQ(5, lines_num);
}
