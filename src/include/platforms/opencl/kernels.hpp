#ifndef __OPENCL_KERNELS_HPP
#define __OPENCL_KERNELS_HPP

#include <string>
#include <stdexcept>

class OpenCLParticleSystem;

class OpenCLKernel {
public:
    virtual void execute() = 0;

    virtual std::string get_source() { return m_source; }

protected:
    std::string m_source;
};

class OpenCLParticleSystemKernel : public OpenCLKernel {
public:
    void set_system(OpenCLParticleSystem* sys) { m_sys = sys; }
protected:
    OpenCLParticleSystem* m_sys;
};

class HelloWorldKernel : public OpenCLKernel {
public:
    HelloWorldKernel();
    virtual void execute();
};

class VerletIntegrationKernel : public OpenCLParticleSystemKernel {
public:
    VerletIntegrationKernel();
    virtual void execute();
};

class EulerIntegrationKernel : public OpenCLParticleSystemKernel {
public:
    EulerIntegrationKernel();
    virtual void execute();
};

class LennardJonesInteractionKernel : public OpenCLParticleSystemKernel {
public:
    LennardJonesInteractionKernel();
    virtual void execute();
};

class IterateLJVerlet : public OpenCLParticleSystemKernel
{
public:
    IterateLJVerlet();
    virtual void execute();
};

#endif // __OPENCL_KERNELS_HPP
