#ifndef __OPENCL_DISPATCHER_H
#define __OPENCL_DISPATCHER_H

#include <memory>

#include "platforms/opencl/opencl_helpers.hpp"
#include "platforms/opencl/opencl_device.hpp"

class OpenCLDispatcher {
public:
    typedef std::shared_ptr<OpenCLDevice> DevicePtr;

    static OpenCLDispatcher& Instance()
    {
        static OpenCLDispatcher self;
        return self;
    }

    template<class Kernel>
    DevicePtr GetDeviceFor(Kernel& kernel)
    {
        // TODO Device selection based on heuristic
        return m_device;
    }

private:
    OpenCLDispatcher()
    {
        // TODO Device selection
        m_device = std::make_shared<OpenCLDevice>(CL_DEVICE_TYPE_ALL);
    }

    DevicePtr m_device;
};

#endif /* __OPENCL_DISPATCHER_H */
