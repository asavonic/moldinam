#include <platforms/opencl/opencl_platform.hpp>

OpenCLParticleSystem::OpenCLParticleSystem()
{
}

OpenCLParticleSystem::OpenCLParticleSystem(ParticleSystemConfig conf) : ParticleSystem(conf)
{
}

NativeParticleSystem OpenCLParticleSystem::convertToNative()
{
    NativeParticleSystem native(m_config);

    native.loadParticles(m_pos.to_native(), m_pos_prev.to_native(),
                            m_vel.to_native(), m_accel.to_native());

    return native;
}

void OpenCLParticleSystem::fromNative(const NativeParticleSystem& native)
{
    m_config = native.config();
    m_pos = cl::float3vec(native.pos().begin(), native.pos().end());
    m_pos_prev = cl::float3vec(native.pos_prev().begin(), native.pos_prev().end());
    m_vel = cl::float3vec(native.vel().begin(), native.vel().end());
    m_accel = cl::float3vec(native.accel().begin(), native.accel().end());
}

void OpenCLParticleSystem::applyVerletIntegration()
{
    OpenCLManager& ocl = OpenCLManager::Instance();
    OpenCLContext context = ocl.getContext();
    VerletIntegrationKernel kernel = context.GetKernel<VerletIntegrationKernel>();

    kernel.set_system(this);

    kernel.execute();
}

void OpenCLParticleSystem::applyEulerIntegration()
{
}
