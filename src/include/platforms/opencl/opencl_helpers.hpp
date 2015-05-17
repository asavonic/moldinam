#ifndef __OPENCL_HELPERS_HPP
#define __OPENCL_HELPERS_HPP

#include <memory>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

#include <platforms/native/types.hpp>
#include <iostream>

class OpenCLContext {
public:
    OpenCLContext() : m_context(CL_DEVICE_TYPE_ALL)
    {
        // TODO: device selection
        // TODO: multiple contexts
    }

    template <class T>
    T GetKernel()
    {
        return T();
    }

    const cl::Context& context() const
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

    const OpenCLContext& getContext() const
    {
        return default_context;
    }

    const cl::CommandQueue& getQueue() const
    {
        return default_queue;
    }

    OpenCLContext default_context;
    cl::CommandQueue default_queue;

private:
    OpenCLManager() : default_queue(default_context.context())
    {
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
        static const ::size_t default_size = 256;

        vector(::size_t size = default_size, cl_mem_flags mem_flags = CL_MEM_READ_WRITE) :
            m_size(size),
            m_queue(OpenCLManager::Instance().getQueue()),
            m_buffer(OpenCLManager::Instance().getContext().context(),
                     mem_flags, sizeof(value_type) * m_size)
        {
        }

        // TODO: need to create or use existing CommandQueue to init m_queue
        // See todo below
        vector(cl::Context context, ::size_t size = default_size, cl_mem_flags mem_flags = CL_MEM_READ_WRITE) :
            m_size(size),
            m_buffer(context, mem_flags, sizeof(value_type) * m_size)
        {
        }

        // TODO: cl::copy will use default queue and it may not match for given context
        // we should either create new queue or use OpenCLContext wrapper
        //
        // NOTE: this may throw -34: invalid context
        template <class IteratorTy>
        vector(cl::Context context, IteratorTy begin, IteratorTy end, cl_mem_flags mem_flags = CL_MEM_READ_WRITE) :
            m_size(std::distance(begin, end)),
            m_buffer(context, mem_flags, sizeof(value_type) * m_size)
        {
            cl::copy(begin, end, m_buffer);
        }

        template <class IteratorTy>
        vector(IteratorTy begin, IteratorTy end, cl_mem_flags mem_flags = CL_MEM_READ_WRITE) :
            m_size(std::distance(begin, end)),
            m_queue(OpenCLManager::Instance().getQueue()),
            m_buffer(OpenCLManager::Instance().getContext().context(),
                     mem_flags, sizeof(value_type) * m_size)
        {
            cl::copy(m_queue, begin, end, m_buffer);
        }

        ~vector()
        {
        }

        inline ::size_t size() { return m_size; }
        inline cl::Buffer& buffer() { return m_buffer; }

        md::float3vec to_native()
        {
            md::float3vec result(m_size);
            cl::copy(m_queue, m_buffer, result.begin(), result.end());
            return result;
        }
    protected:
        ::size_t m_size;
        cl::Buffer m_buffer;
        cl::CommandQueue m_queue;
    };

    using float3vec = cl::vector<cl_float3>;
}


#endif // __OPENCL_HELPERS_HPP
