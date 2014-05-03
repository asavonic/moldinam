#include <md_types.h>

bool operator == ( const double3& first, const double3& second ) {
    return ( first.x == second.x && first.y == second.y && first.z == second.z );
}
bool operator != ( const double3& first, const double3& second ) {
    return !( first == second );
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
