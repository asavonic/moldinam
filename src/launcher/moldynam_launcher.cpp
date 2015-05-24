#include <boost/program_options.hpp>
#include <boost/throw_exception.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <md_algorithms.h>
#include <md_helpers.h>

namespace po = boost::program_options;

int main(int argc, char** argv)
{
    try {
        int iterations = 0;
        std::string config_filename;
        std::string output_filename;
        bool use_stdout = false;
        std::string platform;

        // named arguments
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("iterations", po::value<int>(&iterations)->required(), "number of iterations")
            ("config,c", po::value<std::string>(&config_filename)->required()->multitoken(), "path to particle system config")
            ("output,o", po::value<std::string>(&output_filename), "path to result data file")
            ("stdout", "dump result to stdout")
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

        if (vm.count("stdout")) {
            use_stdout = true;
        }

        po::notify(vm);

        if (platform != "native" && platform != "opencl") {
            throw po::error("invalid value for platform: " + platform);
        }
    } catch (boost::program_options::error& po_error) {
        std::cerr << po_error.what() << std::endl;
    }
    catch (std::exception& ex) {
        std::cerr << "Unknown exception: " << ex.what() << std::endl;
    }
}
