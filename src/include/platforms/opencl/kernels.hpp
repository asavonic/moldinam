#ifndef __OPENCL_KERNELS_HPP
#define __OPENCL_KERNELS_HPP

#include <string>
#include <stdexcept>

class OpenCLParticleSystem;

class OpenCLKernel {
public:
    virtual void execute() = 0;

    std::string get_source() { return m_source; }

protected:
    std::string m_source;
};

class HelloWorldKernel : public OpenCLKernel {
public:
    HelloWorldKernel();
    virtual void execute();
};

class VerletIntegrationKernel : public OpenCLKernel {
public:
    VerletIntegrationKernel();
    virtual void execute();

    std::string get_source() { return m_source; }
    void set_system(OpenCLParticleSystem* sys) { m_sys = sys; }

private:
    OpenCLParticleSystem* m_sys;
};

#endif // __OPENCL_KERNELS_HPP
