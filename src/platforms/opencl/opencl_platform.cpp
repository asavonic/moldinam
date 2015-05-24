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

    std::swap(m_pos, m_pos_prev);
}

void OpenCLParticleSystem::applyEulerIntegration()
{
    OpenCLManager& ocl = OpenCLManager::Instance();
    OpenCLContext context = ocl.getContext();
    EulerIntegrationKernel kernel = context.GetKernel<EulerIntegrationKernel>();

    kernel.set_system(this);

    kernel.execute();

    std::swap(m_pos, m_pos_prev);
}

void OpenCLParticleSystem::loadParticles(std::istream& is, size_t num)
{
    cl::float3vec pos(num);
    cl::float3vec pos_prev(num);
    cl::float3vec vel(num);
    cl::float3vec accel(num);


}

void OpenCLParticleSystem::storeParticles(std::ostream& os)
{
    cl::float3vec::value_type* pos_mapped = nullptr;
    cl::float3vec::value_type* vel_mapped = nullptr;
    cl::float3vec::value_type* accel_mapped = nullptr;

    try {
        using namespace cl;
        pos_mapped = m_pos.map();
        vel_mapped = m_vel.map();
        accel_mapped = m_accel.map();

        for (::size_t i = 0; i < m_pos.size(); i++) {
            os << pos_mapped[i] << " " << vel_mapped[i] << " " << accel_mapped[i] << "\n";
        }
    } catch (...) {
        m_accel.unmap(accel_mapped);
        m_vel.unmap(vel_mapped);
        m_pos.unmap(pos_mapped);

        throw;
    }
}
