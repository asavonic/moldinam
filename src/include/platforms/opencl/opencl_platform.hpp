#ifndef __OPENCL_PLATFORM_HPP
#define __OPENCL_PLATFORM_HPP

#include <platforms/platform.hpp>
#include <platforms/opencl/opencl_helpers.hpp>
#include <platforms/opencl/kernels.hpp>


class OpenCLParticleSystem : public ParticleSystem {
public:
    OpenCLParticleSystem()
    {
    }
        
    explicit OpenCLParticleSystem(ParticleSystemConfig conf) : ParticleSystem(conf)
    {
    }

    virtual void ApplyVerletIntegration()
    {
        OpenCLManager& ocl = OpenCLManager::Instance();
        OpenCLContext context = ocl.GetContext();
        VerletIntegrationKernel kernel = context.GetKernel<VerletIntegrationKernel>();
        kernel.execute();
    }

    virtual void ApplyEulerIntegration()
    {
    }

protected:
    cl::float3vec m_pos;
    cl::float3vec m_pos_prev;
    cl::float3vec m_vel;
    cl::float3vec m_accel;

    cl_float3 m_area_size;
};

#endif // __OPENCL_PLATFORM_HPP
