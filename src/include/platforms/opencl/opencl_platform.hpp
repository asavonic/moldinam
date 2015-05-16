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

    void fromNative(const NativeParticleSystem& native)
    {
        m_config = native.config();
        m_pos = cl::float3vec(native.pos().begin(), native.pos().end());
        m_pos_prev = cl::float3vec(native.pos_prev().begin(), native.pos_prev().end());
        m_vel = cl::float3vec(native.vel().begin(), native.vel().end());
        m_accel = cl::float3vec(native.accel().begin(), native.accel().end());
    }

    virtual void applyVerletIntegration()
    {
        OpenCLManager& ocl = OpenCLManager::Instance();
        OpenCLContext context = ocl.getContext();
        VerletIntegrationKernel kernel = context.GetKernel<VerletIntegrationKernel>();

        kernel.set_system(this);

        kernel.execute();
    }

    virtual void applyEulerIntegration()
    {
    }

    const cl::float3vec& pos() {return m_pos; }
    const cl::float3vec& pos_prev() {return m_pos_prev; }
    const cl::float3vec& vel() {return m_vel; }
    const cl::float3vec& accel() {return m_accel; }

protected:
    cl::float3vec m_pos;
    cl::float3vec m_pos_prev;
    cl::float3vec m_vel;
    cl::float3vec m_accel;
};

#endif // __OPENCL_PLATFORM_HPP
