#ifndef __MD_ALGORITHMS_H
#define __MD_ALGORITHMS_H

#include <md_types.h>
#include <md_helpers.h>
#include <math.h>
#include <stdlib.h>

void Lennard_Jones( double r, LennardJonesConstants& constants, double& force, double& potential );
void verlet( Molecule& mol, double dt );
void euler( Molecule& mol, double dt );
void periodic( Molecule& mol, double3 area_size );
void periodic( std::vector<Molecule>& molecules, double3 area_size );
double distance( Molecule& mol1, Molecule& mol2 );
void simple_interact( Molecule& mol1, Molecule& mol2, LennardJonesConfig& config, bool use_cutoff );
void periodic3d_interact( Molecule& mol1, Molecule& mol2, double3 area_size, LennardJonesConfig& config, bool use_cutoff );


void verlet_step( std::vector<Molecule>& molecules, double dt, LennardJonesConfig& config );
void verlet_step_pariodic( std::vector<Molecule>& molecules, double dt, double3 area_size, LennardJonesConfig& config );
void euler_step( std::vector<Molecule>& molecules, double dt, LennardJonesConfig& config );

#endif
