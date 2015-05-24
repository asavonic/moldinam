#include "gtest/gtest.h"

#include <stdexcept>

#include <config.hpp>
#include "utils.hpp"

TEST(config, lj_psys)
{
    md::ConfigManager& conf_man = md::ConfigManager::Instance();
    conf_man.loadFromFile("config_test.conf");

    md::LennardJonesConfig lj_conf = conf_man.getLennardJonesConfig();
    md::ParticleSystemConfig psys_conf = conf_man.getParticleSystemConfig();

    md::LennardJonesConstants lj_const = lj_conf.getConstants();

    ASSERT_EQ(true, psys_conf.periodic);
    ASSERT_EQ(md::float3(10, 11, 12), psys_conf.area_size.value());

    ASSERT_FLOAT_EQ(0.2, lj_const.get_sigma());
    ASSERT_FLOAT_EQ(0.001, lj_const.get_eps());
}

TEST(config, neg_not_exist)
{
    md::ConfigManager& conf_man = md::ConfigManager::Instance();
    ASSERT_THROW(conf_man.loadFromFile("foobar.conf"), std::runtime_error);
}

TEST(config, neg1)
{
    md::ConfigManager& conf_man = md::ConfigManager::Instance();
    ASSERT_THROW(conf_man.loadFromFile("config_test_neg1.conf"), md::ConfigError);
}

TEST(config, neg2)
{
    md::ConfigManager& conf_man = md::ConfigManager::Instance();
    ASSERT_THROW(conf_man.loadFromFile("config_test_neg2.conf"), md::ConfigError);
}
