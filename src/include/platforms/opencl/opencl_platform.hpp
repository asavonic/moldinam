#ifndef __OPENCL_PLATFORM_HPP
#define __OPENCL_PLATFORM_HPP

#include <CL/cl.hpp>

#include <platforms/platform.hpp>
#include <platforms/opencl/opencl_helpers.hpp>
#include <platforms/opencl/kernels.hpp>

#include <platforms/native/native_platform.hpp>


class OpenCLParticleSystem : public ParticleSystem {
public:
    OpenCLParticleSystem()
    {
    }
        
    explicit OpenCLParticleSystem(ParticleSystemConfig conf) : ParticleSystem(conf)
    {
    }

    NativeParticleSystem convertToNative()
    {
        NativeParticleSystem native(m_config);

        native.loadParticles(m_pos.to_native(), m_pos_prev.to_native(),
                             m_vel.to_native(), m_accel.to_native());

        return native;
    }

    virtual void applyVerletIntegration()
    {
        OpenCLManager& ocl = OpenCLManager::Instance();
        OpenCLContext context = ocl.getContext();
        VerletIntegrationKernel kernel = context.GetKernel<VerletIntegrationKernel>();
        kernel.execute();
    }

    virtual void applyEulerIntegration()
    {
    }

    cl::float3vec& pos() {return m_pos; }
    cl::float3vec& pos_prev() {return m_pos_prev; }
    cl::float3vec& vel() {return m_vel; }
    cl::float3vec& accel() {return m_accel; }

protected:
    cl::float3vec m_pos;
    cl::float3vec m_pos_prev;
    cl::float3vec m_vel;
    cl::float3vec m_accel;

    cl_float3 m_area_size;
};

#endif // __OPENCL_PLATFORM_HPP
