#include <boost/program_options.hpp>
#include <boost/throw_exception.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <utils/config/config_manager.hpp>
#include <platforms/platform.hpp>
#include <platforms/native/native_platform.hpp>
#include <platforms/opencl/opencl_platform.hpp>


namespace po = boost::program_options;

void moldynam(std::vector<std::string> configs, std::string platform, size_t iterations, std::string output);

int main(int argc, char** argv)
{
    try {
        int iterations = 0;
        std::vector<std::string> config_files;
        std::string output_file;
        std::string platform;

        // named arguments
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("iterations", po::value<int>(&iterations)->required(), "number of iterations")
            ("config,c", po::value<std::vector<std::string> >(&config_files)->required()->multitoken(), "path to particle system config")
            ("output,o", po::value<std::string>(&output_file), "path to result data file")
            ("platform,p", po::value<std::string>(&platform)->default_value("native"), "platform usage: native, opencl")
        ;

        // positional arguments
        po::positional_options_description p;
        p.add("input", -1);

        po::variables_map vm;
        po::store(
            po::command_line_parser(argc, argv).options(desc).positional(p).run(),
            vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return 1;
        }

        po::notify(vm);

        if (platform != "native" && platform != "opencl") {
            throw po::error("invalid value for platform: " + platform);
        }

        std::cout << "Selected platform: " << platform << std::endl;
        std::cout << "Iterations: " << iterations << std::endl;
        std::cout << "Output: " << ((output_file == "") ? "none" : output_file) << std::endl;
        std::cout << "Configs: ";
        for (auto& conf : config_files) {
            std::cout << conf << " ";
        }
        std::cout << std::endl;

        moldynam(config_files, platform, iterations, output_file);

    } catch (boost::program_options::error& po_error) {
        std::cerr << po_error.what() << std::endl;
    }
    catch (std::exception& ex) {
        std::cerr << "Unknown exception: " << ex.what() << std::endl;
    }
}

void moldynam(std::vector<std::string> configs, std::string platform, size_t iterations, std::string output)
{
    ConfigManager& conf_man = ConfigManager::Instance();
    for (auto& conf : configs) {
        conf_man.loadFromFile(conf);
    }

    ParticleSystemConfig psys_conf = conf_man.getParticleSystemConfig();

    std::unique_ptr<ParticleSystem> psys;
    if (platform == "native") {
        psys.reset(new NativeParticleSystem(psys_conf));
    } else if (platform == "opencl") {
        psys.reset(new OpenCLParticleSystem(psys_conf));
    }

    psys->setIntegrationAlg(IntegrationAlg::Verlet);
    psys->setPotentialAlg(PotentialAlg::LennardJones);

    psys->iterate(iterations);

    std::ostream* outstream_ptr;
    std::ofstream ofs;
    if (output != "") {
        ofs.open(output);
        outstream_ptr = &ofs;
    } else {
        outstream_ptr = &std::cout;
    }

    psys->storeParticles(*outstream_ptr);
}
