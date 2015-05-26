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

    cl::Program CreateProgram(cl::Program::Sources source, const char* build_options = NULL)
    {
        cl::Program program(m_context, source);
        try {
            program.build(build_options);
        } catch (cl::Error& err) {
            std::cout << "program build failed:\n" << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(m_devices[0]);
            std::cout << std::endl;
            throw;
        }

        return program;
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
