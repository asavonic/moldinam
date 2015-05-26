#ifndef __OPENCL_DEVICE_HPP
#define __OPENCL_DEVICE_HPP

#include "platforms/opencl/opencl_helpers.hpp"
#include <unordered_set>

class OpenCLDevice {
public:
    // TODO handle missing device
    explicit OpenCLDevice(cl_device_type device_type)
    {
        cl_int err = CL_SUCCESS;
        m_context = OpenCLManager::Instance().getContext().context();
        m_devices = m_context.getInfo<CL_CONTEXT_DEVICES>();
        m_queue = cl::CommandQueue(m_context, m_devices[0], 0, &err);
    }

    cl::CommandQueue& get_queue() { return m_queue; }

    cl::Program CreateProgram(std::string source, const char* build_options = NULL)
    {
        cl::Program::Sources src(1, std::make_pair(source.c_str(), source.length()));
        cl::Program program(m_context, src);
        try {
            program.build(build_options);
        } catch (cl::Error& err) {
            std::cout << "program build failed:\n" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices[0]);
            std::cout << std::endl;
            throw;
        }

        return program;
    }

    cl::Program CreateProgram(std::vector<std::string> sources, const char* build_options = NULL)
    {
        std::vector<cl::Program> programs;
        cl::Program linked_program;

        try {
            for (std::string source : sources) {
                cl::Program::Sources src(1, std::make_pair(source.c_str(), source.length()));
                programs.emplace_back(m_context, src);
                programs.back().compile(build_options);
            }
        } catch (cl::Error& err) {
            std::cout << "program build failed:\n";
            for (auto& prog : programs) {
                std::cout << prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices[0]) << std::endl;
            }
            std::cout << std::endl;
            throw;
        }

        // NOTE: cl.hpp have linkPrograms() functions
        // but for some reason it does not allows to specifiy context and uses default one
        // this leads to link failure and some other funny stuff.

        std::vector<cl_program> compiled_progs(programs.size(), 0);
        for (size_t i = 0; i < compiled_progs.size(); ++i) {
            compiled_progs[i] = programs[i]();
        }

        cl_int error = CL_SUCCESS;

        linked_program = ::clLinkProgram(m_context(), 0, NULL, NULL, compiled_progs.size(),
                                         &compiled_progs[0], NULL, NULL, &error);

        if (error != CL_SUCCESS) {
            throw cl::Error(error, "clLinkProgram");
        }

        return linked_program;
    }


protected:
    cl::Context m_context;
    std::vector<cl::Device> m_devices;
    cl::CommandQueue m_queue;

private:
    // Forbid copy
    OpenCLDevice(const OpenCLDevice &rhs) {};
    OpenCLDevice& operator=(const OpenCLDevice &rhs) {return *this;};
};

#endif /* __OPENCL_DEVICE_HPP */
