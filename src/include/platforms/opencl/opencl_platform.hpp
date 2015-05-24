#ifndef __OPENCL_PLATFORM_HPP
#define __OPENCL_PLATFORM_HPP

#include <platforms/platform.hpp>
#include <platforms/opencl/opencl_helpers.hpp>
#include <platforms/opencl/kernels.hpp>

#include <platforms/native/native_platform.hpp>
    
class OpenCLParticleSystem : public ParticleSystem {
public:
    OpenCLParticleSystem();
    explicit OpenCLParticleSystem(ParticleSystemConfig conf);

    NativeParticleSystem convertToNative();
    void fromNative(const NativeParticleSystem& native);
    
    virtual void applyVerletIntegration();
    virtual void applyEulerIntegration();

    virtual void loadParticles(std::istream& is, ::size_t num);
    virtual void storeParticles(std::ostream& os);

    cl::float3vec& pos() { return m_pos; }
    cl::float3vec& pos_prev() { return m_pos_prev; }
    cl::float3vec& vel() { return m_vel; }
    cl::float3vec& accel() { return m_accel; }

protected:
    cl::float3vec m_pos;
    cl::float3vec m_pos_prev;
    cl::float3vec m_vel;
    cl::float3vec m_accel;
}; 

#endif // __OPENCL_PLATFORM_HPP
