#include <sstream>

#include <platforms/opencl/opencl_platform.hpp>
#include <platforms/opencl/opencl_dispatcher.hpp>
#include <platforms/opencl/kernels.hpp>

#include <utils/config/config_manager.hpp>

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

LennardJonesInteractionKernel::LennardJonesInteractionKernel()
{
    m_source = R"(
    float computeLennardJonesForce(float r)
    {
        float ri = 1 / r;
        float ri3 = ri * ri * ri;
        float ri6 = ri3 * ri3;

        // eps and sigma passed as defines

        float force = 48 * eps * ri6 * ri * ri * (sigma_pow_12 * ri6 - sigma_pow_6 / 2);
        return force;
    }

    float computeLennardJonesPotential(float r)
    {
        float ri = 1 / r;
        float ri3 = ri * ri * ri;
        float ri6 = ri3 * ri3;

        // eps and sigma passed as defines
        float potential = 4 * eps * ri6 * (ri6 * sigma_pow_12 - sigma_pow_6);
        return potential;
    }

    float3 computeTargetAccel(float3 target_pos, float3 other_pos)
    {
        float r = distance(target_pos, other_pos);

        float force = computeLennardJonesForce(r);
        float potential = computeLennardJonesPotential(r);

        float3 force_direction = (target_pos - other_pos) / r;
        float3 force_vec = force_direction * force;

        return force_vec;
    }

    __kernel void LennardJonesInteraction(__global float3* pos, __global float3* pos_prev,
                                        __global float3* vel, __global float3* accel, uint num_particles)
    {
        int gid = get_global_id(0);

        for (size_t i = 0; i < gid; ++i) {
            accel[gid] += computeTargetAccel(pos[gid], pos[i]);
        }

        for (size_t i = gid + 1; i < num_particles; ++i) {
            accel[gid] += computeTargetAccel(pos[gid], pos[i]);
        }
    }

    )";
}

void LennardJonesInteractionKernel::execute()
{
    if (!m_sys) {
        throw std::runtime_error("No particle system set to EulerIntegrationKernel");
    }

    OpenCLDispatcher::DevicePtr device =  OpenCLDispatcher::Instance().getDeviceFor(*this);

    cl::Program::Sources src(1, std::make_pair(m_source.c_str(),m_source.length()));

    LennardJonesConfig lj_config = ConfigManager::Instance().getLennardJonesConfig();
    LennardJonesConstants lj_constants = lj_config.getConstants();

    std::stringstream ss;
    ss << " -Deps=" << lj_constants.get_eps<float>();
    ss << " -Dsigma=" << lj_constants.get_sigma<float>();
    ss << " -Dsigma_pow_6=" << lj_constants.get_sigma_pow_6<float>();
    ss << " -Dsigma_pow_12=" << lj_constants.get_sigma_pow_12<float>();

    cl::Program program = device->CreateProgram(src, ss.str().c_str());
    cl::Kernel kernel(program, "LennardJonesInteraction");

    kernel.setArg(0, m_sys->pos().buffer()());
    kernel.setArg(1, m_sys->pos_prev().buffer()());
    kernel.setArg(2, m_sys->vel().buffer()());
    kernel.setArg(3, m_sys->accel().buffer()());

    cl_uint size = m_sys->pos().size();

    kernel.setArg(4, size);


    cl::Event event;
    device->get_queue().enqueueNDRangeKernel(kernel, cl::NDRange(0),
                                            cl::NDRange(size), cl::NDRange(), NULL, &event);
    event.wait();
}

IterateLJVerlet::IterateLJVerlet()
{
    m_source = R"(
    void LennardJonesInteraction(__global float3* pos, __global float3* pos_prev,
                                 __global float3* vel, __global float3* accel, uint num_particles);

    void VerletIntegration(__global float3* pos, __global float3* pos_prev,
                           __global float3* accel, float dt);

    __kernel IterateLJVerlet(__global float3* pos, __global float3* pos_prev,
                               __global float3* vel, __global float3* accel, 
                               uint num_particles, float dt)
    {
        EulerIntegration(pos, pos_prev, vel, accel, dt);
        for (size_t i = 0; i < ITERATIONS; ++i) {
            LennardJonesInteraction(pos, pos_prev, vel, accel, num_particles);
            VerletIntegration(pos, pos_prev, vel, accel, dt);

            work_group_barrier(CLK_GLOBAL_MEM_FENCE);

            float3* tmp = pos;
            pos = pos_prev;
            pos_prev = tmp;
        }
    }
)";
}

void IterateLJVerlet::execute()
{
    if (!m_sys) {
        throw std::runtime_error("No particle system set to EulerIntegrationKernel");
    }

    OpenCLDispatcher::DevicePtr device =  OpenCLDispatcher::Instance().getDeviceFor(*this);

    cl::Program::Sources src(1, std::make_pair(m_source.c_str(),m_source.length()));

    LennardJonesConfig lj_config = ConfigManager::Instance().getLennardJonesConfig();
    LennardJonesConstants lj_constants = lj_config.getConstants();

    std::stringstream ss;
    ss << " -Deps=" << lj_constants.get_eps<float>();
    ss << " -Dsigma=" << lj_constants.get_sigma<float>();
    ss << " -Dsigma_pow_6=" << lj_constants.get_sigma_pow_6<float>();
    ss << " -Dsigma_pow_12=" << lj_constants.get_sigma_pow_12<float>();

    cl::Program program = device->CreateProgram(src, ss.str().c_str());
    cl::Kernel kernel(program, "IterateLJVerlet");

    kernel.setArg(0, m_sys->pos().buffer()());
    kernel.setArg(1, m_sys->pos_prev().buffer()());
    kernel.setArg(2, m_sys->vel().buffer()());
    kernel.setArg(3, m_sys->accel().buffer()());

    cl_uint size = m_sys->pos().size();
    kernel.setArg(4, size);

    cl_float dt = (float) m_sys->config().dt;
    kernel.setArg(5, dt);

    cl::Event event;
    device->get_queue().enqueueNDRangeKernel(kernel, cl::NDRange(0),
                                            cl::NDRange(size), cl::NDRange(), NULL, &event);
    event.wait();
    
}
