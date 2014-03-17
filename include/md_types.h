#ifndef __MD_TYPES_H
#define __MD_TYPES_H

#include <vector>

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

enum Molecule_Type {
    H,
    O,
    NONE_TYPE
};

struct Molecule {
    double3 pos;
    double3 pos_prev;
    double3 speed;
    double3 accel;
    Molecule_Type type;
};

struct Molecule_Field  {
    std::vector< Molecule > molecules;
};

#endif
