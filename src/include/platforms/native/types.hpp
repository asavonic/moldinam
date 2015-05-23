#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <sstream>

namespace md {
    typedef glm::vec3 float3;
    typedef std::vector<float3> float3vec;

    using glm::floor;
    using glm::distance;

    std::istream& operator>>(std::istream& is, float3& f3);
    std::ostream& operator<<(std::ostream& os, const float3& f3);
}
