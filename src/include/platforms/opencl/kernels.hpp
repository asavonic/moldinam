#ifndef __OPENCL_KERNELS_HPP
#define __OPENCL_KERNELS_HPP

class OpenCLKernel {
public:
    virtual void execute() = 0;
};

class VerletIntegrationKernel : public OpenCLKernel {
public:
    virtual void execute()
    {
        // TODO
    }
};

#endif // __OPENCL_KERNELS_HPP
