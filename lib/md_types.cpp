#include <md_types.h>

bool operator == ( const double3& first, const double3& second ) {
    return ( first.x == second.x && first.y == second.y && first.z == second.z );
}
bool operator != ( const double3& first, const double3& second ) {
    return !( first == second );
}

double3 operator*( const double3& a, const float k ) {
    double3 result;
    result.x = a.x * k;
    result.y = a.y * k;
    result.z = a.z * k;

    return result;
}
double3 operator/( const double3& a, const float k ) {
    double3 result;
    result.x = a.x / k;
    result.y = a.y / k;
    result.z = a.z / k;

    return result;
}
double3 operator+( const double3& a, const double3& b ) {
    double3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;
}
double3 operator-( const double3& a, const double3& b ) {
    double3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}

void operator+=( double3& a, const double3& b ) {
    a.x = a.x + b.x;
    a.y = a.y + b.y;
    a.z = a.z + b.z;
}
void operator-=( double3& a, const double3& b ) {
    a.x = a.x - b.x;
    a.y = a.y - b.y;
    a.z = a.z - b.z;
}

bool operator == ( const Molecule& first, const Molecule& second ) {
    return ( first.pos      == second.pos      && 
             first.pos_prev == second.pos_prev &&
             first.speed    == second.speed    &&
             first.accel    == second.accel    &&
             first.type     == second.type );
}
bool operator != ( const Molecule& first, const Molecule& second) {
    return !( first == second );
}
