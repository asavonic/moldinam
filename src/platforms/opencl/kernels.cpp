#include <platforms/opencl/opencl_platform.hpp>
#include <platforms/opencl/opencl_dispatcher.hpp>
#include <platforms/opencl/kernels.hpp>

HelloWorldKernel::HelloWorldKernel()
{
    m_source = R"(__kernel void print() { printf("hello world!\n");})";
}

void HelloWorldKernel::execute()
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


VerletIntegrationKernel::VerletIntegrationKernel()
{
    m_source = R"(
        __kernel void VerletIntegration(__global float3* pos, __global float3* pos_prev,
                                        __global float3* accel, float dt)
        {
            int gid = get_global_id(0);
            pos_prev[gid] = 2 * pos[gid] - pos_prev[gid] + accel[gid] * dt * dt;
            // swap pos_prev and pos required after execution!
        }
    )";
}

void VerletIntegrationKernel::execute()
{
    if (!m_sys) {
        throw std::runtime_error("No particle system set to VerletIntegrationKernel");
    }

    OpenCLDispatcher::DevicePtr device =  OpenCLDispatcher::Instance().getDeviceFor(*this);

    cl::Program::Sources src(1, std::make_pair(m_source.c_str(),m_source.length()));

    cl::Program program = device->CreateProgram(src);
    cl::Kernel kernel(program, "VerletIntegration");

    kernel.setArg(0, m_sys->pos().buffer()());
    kernel.setArg(1, m_sys->pos_prev().buffer()());
    kernel.setArg(2, m_sys->accel().buffer()());

    cl_float dt = (float) m_sys->config().dt;
    kernel.setArg(3, dt);

    size_t size = m_sys->pos().size();

    cl::Event event;
    device->get_queue().enqueueNDRangeKernel(kernel, cl::NDRange(0),
                                            cl::NDRange(size), cl::NDRange(), NULL, &event);
    event.wait();
}

EulerIntegrationKernel::EulerIntegrationKernel()
{
    m_source = R"(
        __kernel void EulerIntegration(__global float3* pos, __global float3* pos_prev,
                                       __global float3* vel, __global float3* accel, float dt)
        {
            int gid = get_global_id(0);
            pos_prev[gid] = pos[gid] + vel[gid] * dt + accel[gid] * dt * dt;
            // swap pos_prev and pos required after execution!
        }
    )";
}

void EulerIntegrationKernel::execute()
{
    if (!m_sys) {
        throw std::runtime_error("No particle system set to EulerIntegrationKernel");
    }

    OpenCLDispatcher::DevicePtr device =  OpenCLDispatcher::Instance().getDeviceFor(*this);

    cl::Program::Sources src(1, std::make_pair(m_source.c_str(),m_source.length()));

    cl::Program program = device->CreateProgram(src);
    cl::Kernel kernel(program, "EulerIntegration");

    kernel.setArg(0, m_sys->pos().buffer()());
    kernel.setArg(1, m_sys->pos_prev().buffer()());
    kernel.setArg(2, m_sys->vel().buffer()());
    kernel.setArg(3, m_sys->accel().buffer()());

    cl_float dt = (float) m_sys->config().dt;
    kernel.setArg(4, dt);

    size_t size = m_sys->pos().size();

    cl::Event event;
    device->get_queue().enqueueNDRangeKernel(kernel, cl::NDRange(0),
                                            cl::NDRange(size), cl::NDRange(), NULL, &event);
    event.wait();
}
