#include <platforms/opencl/opencl_helpers.hpp>
#include <platforms/native/types.hpp>

namespace cl {
    
cl_type_t<md::float3> convert_to_cl(const md::float3& rhs )
{
    cl_type_t<md::float3> lhs;

    lhs.s[0] = rhs.x;
    lhs.s[1] = rhs.y;
    lhs.s[2] = rhs.z;

    return lhs;
}

template <>
md::float3 convert_to(const cl_float3& rhs)
{
    return md::float3(rhs.s[0], rhs.s[1], rhs.s[2]);
}

std::ostream& operator<<(std::ostream& os, const cl_float3& f3)
{
    os << f3.s[0] << " " << f3.s[1] << " " << f3.s[2];
    return os;
}
std::istream& operator>>(std::istream& is, cl_float3& f3)
{
    is >> f3.s[0] >> f3.s[1] >> f3.s[2];
    return is;
}
    
} // namespace cl
