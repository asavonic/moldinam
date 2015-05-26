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
    T GetKernel() const
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
    std::ostream& operator<<(std::ostream& os, const cl_float3& f3);
    std::istream& operator>>(std::istream& is, cl_float3& f3);


    template <typename T>
    struct cl_type_traits {
    };

    template <>
    struct cl_type_traits<cl_float3> {
        typedef md::float3 native_type;
        typedef cl_float3 cl_type;
    };

    template <>
    struct cl_type_traits<md::float3> {
        typedef cl_float3 cl_type;
    };

    template <typename T>
    using cl_native_type_t = typename cl_type_traits<T>::native_type;

    template <typename T>
    using cl_type_t = typename cl_type_traits<T>::cl_type;


    cl_type_t<md::float3> convert_to_cl(const md::float3& rhs );

    template <typename T, typename CLType>
    T convert_to(const CLType& rhs);

    /**
    * Blocking copy operation between iterators and a buffer.
    * Performs conversions to apropriate type before copy using cl_type_traits.
    * Host to Device.
    * Uses specified queue.
    */
    template< typename IteratorType >
    inline cl_int conv_copy( const CommandQueue &queue, IteratorType startIterator, IteratorType endIterator, cl::Buffer &buffer )
    {
        typedef typename std::iterator_traits<IteratorType>::value_type iter_data_type;
        typedef cl_type_t<iter_data_type> data_type;

        cl_int error;

        ::size_t length = endIterator-startIterator;
        ::size_t byteLength = length*sizeof(data_type);

        data_type* pointer = 
            static_cast<data_type*>(queue.enqueueMapBuffer(buffer, CL_TRUE, CL_MAP_WRITE, 0, byteLength, 0, 0, &error));
        // if exceptions enabled, enqueueMapBuffer will throw
        if( error != CL_SUCCESS ) {
            return error;
        }

        // perform copy with convertion
        data_type* dest = pointer;
        while (startIterator != endIterator) {
            *dest++ = convert_to_cl(*startIterator++);
        }

        Event endEvent;
        error = queue.enqueueUnmapMemObject(buffer, pointer, 0, &endEvent);
        // if exceptions enabled, enqueueUnmapMemObject will throw
        if( error != CL_SUCCESS ) { 
            return error;
        }
        endEvent.wait();
        return CL_SUCCESS;
    }

    /**
    * Blocking copy operation between iterators and a buffer.
    * Performs conversions to apropriate type before copy using cl_type_traits.
    * Device to Host.
    * Uses specified queue.
    */
    template< typename IteratorType >
    inline cl_int conv_copy( const CommandQueue &queue, cl::Buffer &buffer, IteratorType startIterator, IteratorType endIterator)
    {
        typedef typename std::iterator_traits<IteratorType>::value_type iter_data_type;
        typedef cl_type_t<iter_data_type> data_type;

        cl_int error;

        ::size_t length = endIterator-startIterator;
        ::size_t byteLength = length*sizeof(data_type);

        data_type* pointer = 
            static_cast<data_type*>(queue.enqueueMapBuffer(buffer, CL_TRUE, CL_MAP_WRITE, 0, byteLength, 0, 0, &error));
        // if exceptions enabled, enqueueMapBuffer will throw
        if( error != CL_SUCCESS ) {
            return error;
        }

        // perform copy with convertion
        data_type* dest = pointer;
        while (startIterator != endIterator) {
            *startIterator++ = convert_to<iter_data_type>(*dest++);
        }

        Event endEvent;
        error = queue.enqueueUnmapMemObject(buffer, pointer, 0, &endEvent);
        // if exceptions enabled, enqueueUnmapMemObject will throw
        if( error != CL_SUCCESS ) { 
            return error;
        }
        endEvent.wait();
        return CL_SUCCESS;
    }


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
            cl::conv_copy(m_queue, begin, end, m_buffer);
        }

        ~vector()
        {
        }

        inline ::size_t size() const { return m_size; }
        inline cl::Buffer& buffer() { return m_buffer; }

        md::float3vec to_native()
        {
            md::float3vec result(m_size);
            cl::conv_copy(m_queue, m_buffer, result.begin(), result.end());
            return result;
        }

        value_type* map()
        {
            ::size_t size = sizeof(value_type) * m_size;
            value_type* pointer = 
                static_cast<value_type*>(m_queue.enqueueMapBuffer(m_buffer, CL_TRUE, CL_MAP_WRITE, 0, size, 0, 0, nullptr));
            return pointer;
        }

        void unmap(value_type* ptr)
        {
            Event end;
            m_queue.enqueueUnmapMemObject(m_buffer, ptr, 0, &end);
            end.wait();
        }
    protected:
        ::size_t m_size;
        cl::Buffer m_buffer;
        cl::CommandQueue m_queue;
    };

    using float3vec = cl::vector<cl_float3>;
}


#endif // __OPENCL_HELPERS_HPP
