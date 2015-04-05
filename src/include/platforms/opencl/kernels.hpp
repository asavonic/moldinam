#ifndef __OPENCL_KERNELS_HPP
#define __OPENCL_KERNELS_HPP

#include <string>
#include <stdexcept>

#include <platforms/opencl/opencl_dispatcher.hpp>


class OpenCLKernel {
public:
    virtual void execute() = 0;

    std::string get_source() { return m_source; }

protected:
    std::string m_source;
};

class HelloWorldKernel : public OpenCLKernel {
public:
    HelloWorldKernel()
    {
        m_source = R"(__kernel void print() { printf("hello world!\n");})";
    }

    virtual void execute()
    {
        // TODO Implement common solution like cl::KernelFunctor from newest cl.hpp
        // with arguments binding to avoid copy/paste this init stuff

        OpenCLDispatcher::DevicePtr device =  OpenCLDispatcher::Instance().getDeviceFor(*this);

        cl::Program::Sources src(1, std::make_pair(m_source.c_str(),m_source.length()));

        cl::Program program = device->CreateProgram(src);
        cl::Kernel kernel(program, "print");

        cl::Event event;
        device->get_queue().enqueueNDRangeKernel(kernel, cl::NDRange(0),
                                             cl::NDRange(4,4), cl::NDRange(2,2), NULL, &event);

        event.wait();
    }

};

class VerletIntegrationKernel : public OpenCLKernel {
public:
    VerletIntegrationKernel()
    {
        m_source = R"(
            __kernel void VerletIntegration(__global float3* pos, __global float3* pos_prev,
                                            __global float3* accel, int dt)
            {
                int gid = get_global_id(0);
                pos[gid].x = 2 * pos[gid].x - pos_prev[gid].x + accel[gid].x * dt * dt;
            }
        )";
    }

    virtual void execute()
    {
        OpenCLDispatcher::DevicePtr device =  OpenCLDispatcher::Instance().getDeviceFor(*this);

        cl::Program::Sources src(1, std::make_pair(m_source.c_str(),m_source.length()));

        cl::Program program = device->CreateProgram(src);
        cl::Kernel kernel(program, "print");

        cl::Event event;
        device->get_queue().enqueueNDRangeKernel(kernel, cl::NDRange(0),
                                             cl::NDRange(4,4), cl::NDRange(2,2), NULL, &event);

        event.wait();
    }

    std::string get_source() { return m_source; }
};

#endif // __OPENCL_KERNELS_HPP
