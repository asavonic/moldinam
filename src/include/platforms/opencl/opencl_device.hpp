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
        m_context = cl::Context(device_type, 0, NULL, NULL, &err); 
        m_devices = m_context.getInfo<CL_CONTEXT_DEVICES>();
        m_queue = cl::CommandQueue(m_context, m_devices[0], 0, &err);
    }

    cl::CommandQueue& get_queue() { return m_queue; }

    cl::Program CreateProgram(cl::Program::Sources source)
    {
       cl::Program program(m_context, source);
       program.build();
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
