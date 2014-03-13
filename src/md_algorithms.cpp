#include <math.h>
#include <md_algorithms.h>

void Lennard_Jones( double r, double epsilon, double sigma, double& force, double& potential ) {
    double ri = 1 / r;
    double ri3 = ri * ri * ri;
    double ri6 = ri3 * ri3;
    
    force = 48 * epsilon * ( pow( sigma, 12 ) * ri6 - pow( sigma, 6 ) / 2 ) * ri6 * ri * ri;
    potential = 4 * epsilon * ri6 * ( ri6 * pow( sigma, 12 ) - pow( sigma, 6 ) );
}

inline void verlet( Molecule& mol, double dt ) {
    mol.pos_prev.x = 2 * mol.pos.x - mol.pos_prev.x + mol.accel.x * dt * dt;                            
    mol.pos_prev.y = 2 * mol.pos.y - mol.pos_prev.y + mol.accel.y * dt * dt;                            
    mol.pos_prev.z = 2 * mol.pos.z - mol.pos_prev.z + mol.accel.z * dt * dt;                            

    std::swap( mol.pos_prev, mol.pos );
}

inline void euler( Molecule& mol, double dt ) {
    std::swap( mol.pos_prev, mol.pos );

    mol.pos.x = mol.pos_prev.x + mol.speed.x * dt;
    mol.pos.y = mol.pos_prev.y + mol.speed.y * dt;
    mol.pos.z = mol.pos_prev.z + mol.speed.z * dt;
}
