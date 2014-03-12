#include <vector>

struct double3 {
    double x;
    double y;
    double z;

    double3() {
        x = y = z = 0;
    } 
};

enum Molecule_Type {
    H,
    O,
    NONE_TYPE
};

struct Molecule {
    double3 pos;
    double3 speed;
    double3 accel;
    Molecule_Type type;
};

struct Molecule_Field  {
    std::vector< Molecule > molecules;
};
