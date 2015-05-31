#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <sstream>

namespace md {
    typedef glm::vec3 float3;
    typedef std::vector<float3> float3vec;

    using glm::floor;
    using glm::distance;

    inline float sqr_distance(const float3& lhs, const float3& rhs)
    {
        float dx = lhs.x - rhs.x;
        float dy = lhs.y - rhs.y;
        float dz = lhs.z - rhs.z;

        return dx * dx + dy * dy + dz * dz;
    }

    std::istream& operator>>(std::istream& is, float3& f3);
    std::ostream& operator<<(std::ostream& os, const float3& f3);
}
