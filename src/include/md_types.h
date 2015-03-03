#ifndef __MD_TYPES_H
#define __MD_TYPES_H

#include <vector>
#include <string>

struct double3 {
    double x;
    double y;
    double z;

    double3() {
        x = y = z = 0;
    } 
    double3( double _x, double _y, double _z ) {
        x = _x;
        y = _y;
        z = _z;
    }
};

bool operator == ( const double3&, const double3& );
bool operator != ( const double3&, const double3& );
double3 operator*( const double3&, const float );
double3 operator/( const double3&, const float );
double3 operator+( const double3&, const double3& );
double3 operator-( const double3&, const double3& );
void operator+=( double3&, const double3& );
void operator-=( double3&, const double3& );

enum class Molecule_Type {
    NO_TYPE = 0,
    H = 1 << 0,
    O = 1 << 1,
};

struct Molecule {
    double3 pos;
    double3 pos_prev;
    double3 speed;
    double3 accel;
    Molecule_Type type;
};

bool operator == ( const Molecule&, const Molecule& );
bool operator != ( const Molecule&, const Molecule& );

struct Molecule_Field  {
    std::vector< Molecule > molecules;
};

#endif
