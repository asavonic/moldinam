#ifndef __MD_ALGORITHMS_H
#define __MD_ALGORITHMS_H

#include <md_types.h>

void Lennard_Jones( double r, double epsilon, double sigma, double& force, double& potential );
inline void verlet( Molecule& mol, double dt );
inline void euler( Molecule& mol, double dt );


#endif
