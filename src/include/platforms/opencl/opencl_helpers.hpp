#ifndef __OPENCL_HELPERS_HPP
#define __OPENCL_HELPERS_HPP

#include <memory>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

class OpenCLContext {
public:
    OpenCLContext() : m_context(cl::Context::getDefault())
    {
        // TODO: device selection
        // TODO: multiple contexts
    }

    template <class T>
    T GetKernel()
    {
        return T();
    }

protected:
    cl::Context m_context;
};

class OpenCLManager {
public:
    static OpenCLManager& Instance()
    {
        static OpenCLManager m_instance;
        return m_instance;
    }

    OpenCLContext GetContext()
    {
        return OpenCLContext();
    }
};


namespace cl {
    struct float3vec {
        cl::Buffer x_vec;
        cl::Buffer y_vec;
        cl::Buffer z_vec;
    };
}


#endif // __OPENCL_HELPERS_HPP
