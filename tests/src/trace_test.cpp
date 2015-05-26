#include "gtest/gtest.h"

#include <stdexcept>

#include <platforms/trace.hpp>
#include <platforms/native/native_platform.hpp>

#include "utils.hpp"

TEST(trace, simple)
{
    NativeParticleSystem p_sys;
    TraceCollector trace(std::cout);
    trace.attach(p_sys);

    p_sys.invokeOnIteration(1);
}
