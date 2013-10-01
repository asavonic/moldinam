#ifndef __DINAMIC_H
#define __DINAMIC_H

#include "types.h"
#include "math.h"



enum InteractionMode
{
	DEFAULT,
	PERIODIC_SOUTH,
	PERIODIC_SOUTH_WEST,
	PERIODIC_SOUTH_EAST,
	PERIODIC_NORTH,
	PERIODIC_NORTH_WEST,
	PERIODIC_NORTH_EAST,
	PERIODIC_WEST,
	PERIODIC_EAST,
};
typedef enum InteractionMode InteractionMode; 

float ComputeForce(float r, float sigma, float eps);

float GetDistance(Molecule* firstMol, Molecule* secondMol, 
								float sizeX, float sizeY, InteractionMode mode);


void ModifyForce(Molecule* firstMol, Molecule* secondMol, 
								float sizeX, float sizeY, InteractionMode mode);


void Interact(Molecule* firstMol, Molecule* secondMol,
								float sizeX, float sizeY, InteractionMode mode);

void ClearForceAccel(Cell* cell);

void MakeInteractions(Cell* cell);

void MethodEuler(Molecule* mol, float dt);

void MethodVerlet(Molecule* mol, float dt);

void CorrectPosition(Molecule* mol, float sizeX, float sizeY, float sizeZ);

void RecalculateParametres(Cell* cell, float dt);

void DinamicIterate(Cell* cell, float dt);

void DinamicInit(Cell* cell, float dt);


#endif