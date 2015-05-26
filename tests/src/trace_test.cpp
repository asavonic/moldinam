#include "gtest/gtest.h"

#include <stdexcept>

#include <utils/trace.hpp>
#include <platforms/native/native_platform.hpp>

#include "utils.hpp"

TEST(trace, simple)
{
    NativeParticleSystem p_sys;
    TraceCollector trace;
    trace.attach(p_sys);

    p_sys.invokeOnIteration(1);
}
