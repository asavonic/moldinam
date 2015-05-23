#include <platforms/native/types.hpp>

namespace md {
    
std::istream& operator>>(std::istream& is, float3& f3)
{
    is >> f3.x >> f3.y >> f3.z;
    return is;
}

std::ostream& operator<<(std::ostream& os, const float3& f3)
{
    os << f3.x << f3.y << f3.z;
    return os;
}

} // namespace md

