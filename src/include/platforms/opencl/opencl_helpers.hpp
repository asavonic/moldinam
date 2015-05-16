#ifndef __OPENCL_HELPERS_HPP
#define __OPENCL_HELPERS_HPP

#include <memory>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <platforms/native/types.hpp>

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

    const cl::Context& context()
    {
        return m_context;
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

    OpenCLContext getContext()
    {
        return OpenCLContext();
    }
};


namespace cl {

    template <typename T>
    struct cl_type_traits {
    };

    template <>
    struct cl_type_traits<cl_float3> {
        typedef md::float3 native_type;
    };

    template <typename T>
    using cl_native_type_t = typename cl_type_traits<T>::native_type;


    // TODO: implement cl::vector
    template <typename T>
    class vector{
    public:
        using value_type = T;
        using native_value_type = cl_native_type_t<T>;

        vector(::size_t size = 0, cl_mem_flags mem_flags = CL_MEM_READ_WRITE) :
            m_buffer(OpenCLManager::Instance().getContext().context(), size, mem_flags)
        {
        }

        vector(cl::Context context, ::size_t size = 0, cl_mem_flags mem_flags = CL_MEM_READ_WRITE) :
            m_buffer(context, size, mem_flags)
        {
        }

        template <class IteratorTy>
        vector(cl::Context context, IteratorTy begin, IteratorTy end) :
            m_buffer(context, begin, end, /*read only*/ false)
        {
        }

        template <class IteratorTy>
        vector(IteratorTy begin, IteratorTy end) :
            m_buffer(OpenCLManager::Instance().getContext().context(), begin, end, /*read only*/ false),
            m_size(std::distance(begin, end))
        {
        }

        ~vector()
        {
        }

        inline ::size_t size() { return m_size; }

        md::float3vec to_native()
        {
            md::float3vec result(m_size);
            cl::copy(m_buffer, result.begin(), result.end());
            return result;
        }
    protected:
        ::size_t m_size;
        cl::Buffer m_buffer;
    };

    using float3vec = cl::vector<cl_float3>;
}


#endif // __OPENCL_HELPERS_HPP
