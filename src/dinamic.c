#include "include/dinamic.h"

float ComputeForce(float r, float sigma, float eps)
{
	float ri = sigma/r;
    float ri3 = ri*ri*ri;
    float ri6 = ri3 * ri3;
    float g = 24 * ri * ri6 * ( 2 * ri6 - 1 );
    float force = g / r;

    return force; 
}


float GetDistance(Molecule* firstMol, Molecule* secondMol, 
								float sizeX, float sizeY, InteractionMode mode)
{
	float dx = firstMol->pos[0] - secondMol->pos[0];
	float dy = firstMol->pos[1] - secondMol->pos[1];
	float dz = firstMol->pos[2] - secondMol->pos[2];

	switch (mode)
	{
		case DEFAULT : { break; }
		case PERIODIC_NORTH : { dy += sizeY; break; }
		case PERIODIC_NORTH_EAST : { dy += sizeY; dx += sizeX; break; }
		case PERIODIC_NORTH_WEST : { dy += sizeY; dx -= sizeX; break; }
		case PERIODIC_SOUTH : { dy -= sizeY; break; }
		case PERIODIC_SOUTH_EAST : { dy -= sizeY; dx += sizeX; break; }
		case PERIODIC_SOUTH_WEST : { dy -= sizeY; dx -= sizeX; break; }
		case PERIODIC_EAST : { dx += sizeX; break; }
		case PERIODIC_WEST : { dx -= sizeX; break; }
	}

	return sqrt ( dx*dx + dy*dy + dz*dz );
}


void ModifyForce(Molecule* firstMol, Molecule* secondMol, 
								float sizeX, float sizeY, InteractionMode mode)
{
	// refactoring needs here...
	float r = GetDistance(firstMol, secondMol, sizeX, sizeY, mode);
	if (r > sizeX/2 || r > sizeY/2)
	{
		return; // distance between molecules is too big, no need to calculate force
	}
	float sigma = 1.0;
	float eps = 1.0;
	float force = ComputeForce(r,sigma,eps);

	float dx = firstMol->pos[0] - secondMol->pos[0];
	float dy = firstMol->pos[1] - secondMol->pos[1];
	float dz = firstMol->pos[2] - secondMol->pos[2];

	firstMol->force[0] += force*dx;
	firstMol->force[1] += force*dy;
	firstMol->force[2] += force*dz;

	if (mode == DEFAULT) // no need to modify second molecule if it`s periodic cell
	{
		secondMol->force[0] += -force*dx;
		secondMol->force[1] += -force*dy;
		secondMol->force[2] += -force*dz;
	}
}

void Interact(Molecule* firstMol, Molecule* secondMol,
								float sizeX, float sizeY, InteractionMode mode)
{
	ModifyForce(firstMol, secondMol, sizeX, sizeY, mode);
	// need to add Potential energy computing
	// !!!
}

void ClearForceAccel(Cell* cell)
{
	for (int i = 0; i < cell->molNum; ++i)
	{
		cell->molecules[i]->force[0] = 0;
		cell->molecules[i]->force[1] = 0;
		cell->molecules[i]->force[2] = 0;
		cell->molecules[i]->accel[0] = 0;
		cell->molecules[i]->accel[1] = 0;
		cell->molecules[i]->accel[2] = 0;
	}
}

void MakeInteractions(Cell* cell)
{
	float sizeX = cell->sizeX;
	float sizeY = cell->sizeY;

	// make pair interactions
	for (int i = 0; i < cell->molNum - 1; ++i)
	{
		for (int j = i + 1; j < cell->molNum; ++j)
		{
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, DEFAULT);
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, PERIODIC_SOUTH);
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, PERIODIC_NORTH);
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, PERIODIC_WEST);
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, PERIODIC_EAST);
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, PERIODIC_SOUTH_WEST);
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, PERIODIC_SOUTH_EAST);
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, PERIODIC_NORTH_WEST);
			Interact(cell->molecules[i], cell->molecules[j], sizeX, sizeY, PERIODIC_NORTH_EAST);
		}
	}
	// make interactions with one same molecule but in periodic cells
	for (int i = 0; i < cell->molNum; ++i)
	{
		Interact(cell->molecules[i], cell->molecules[i], sizeX, sizeY, PERIODIC_SOUTH);
		Interact(cell->molecules[i], cell->molecules[i], sizeX, sizeY, PERIODIC_NORTH);
		Interact(cell->molecules[i], cell->molecules[i], sizeX, sizeY, PERIODIC_WEST);
		Interact(cell->molecules[i], cell->molecules[i], sizeX, sizeY, PERIODIC_EAST);
		Interact(cell->molecules[i], cell->molecules[i], sizeX, sizeY, PERIODIC_SOUTH_WEST);
		Interact(cell->molecules[i], cell->molecules[i], sizeX, sizeY, PERIODIC_SOUTH_EAST);
		Interact(cell->molecules[i], cell->molecules[i], sizeX, sizeY, PERIODIC_NORTH_WEST);
		Interact(cell->molecules[i], cell->molecules[i], sizeX, sizeY, PERIODIC_NORTH_EAST);
	}
}

void MethodEuler(Molecule* mol, float dt)
{
	mol->accel[0] = mol->force[0];
	mol->accel[1] = mol->force[1];
	mol->accel[2] = mol->force[2];


	mol->speed[0] += mol->accel[0];
	mol->speed[1] += mol->accel[1];
	mol->speed[2] += mol->accel[2];

	mol->prevpos[0] = mol->pos[0];
	mol->prevpos[1] = mol->pos[1];
	mol->prevpos[2] = mol->pos[2];

	mol->pos[0] += ( mol->speed[0] + 0.5 * dt * mol->accel[0] ) * dt;
	mol->pos[1] += ( mol->speed[1] + 0.5 * dt * mol->accel[1] ) * dt;
	mol->pos[2] += ( mol->speed[2] + 0.5 * dt * mol->accel[2] ) * dt;
}

void MethodVerlet(Molecule* mol, float dt)
{
	mol->accel[0] = mol->force[0];
	mol->accel[1] = mol->force[1];
	mol->accel[2] = mol->force[2];

	float nextpos[3];
	nextpos[0] = 2 * mol->pos[0] - mol->prevpos[0] + dt * dt * mol->accel[0];
	nextpos[1] = 2 * mol->pos[1] - mol->prevpos[1] + dt * dt * mol->accel[1];
	nextpos[2] = 2 * mol->pos[2] - mol->prevpos[2] + dt * dt * mol->accel[2];

	mol->prevpos[0] = mol->pos[0];
	mol->prevpos[1] = mol->pos[1];
	mol->prevpos[2] = mol->pos[2];

	mol->pos[0] = nextpos[0];
	mol->pos[1] = nextpos[1];
	mol->pos[2] = nextpos[2];
}

void CorrectPosition(Molecule* mol, float sizeX, float sizeY, float sizeZ)
{// is there a better way?
	if (mol->pos[0] > sizeX)
	{
		mol->pos[0] = mol->pos[0] - sizeX;
	}
	if (mol->pos[1] > sizeY)
	{
		mol->pos[1] = mol->pos[1] - sizeY;
	}
	if (mol->pos[2] > sizeZ)
	{
		mol->pos[2] = mol->pos[2] - 2 * ( mol->pos[2] - sizeZ ) ; // REWRITE HERE!!!!!!!
	}
	if (mol->pos[0] < 0)
	{
		mol->pos[0] = mol->pos[0] + sizeX;
	}
	if (mol->pos[1] < 0)
	{
		mol->pos[1] = mol->pos[1] + sizeY;
	}
	if (mol->pos[2] < 0)
	{
		mol->pos[2] = - mol->pos[2]; //hell, it`s a hardcode
	}
}
void RecalculateParametres(Cell* cell, float dt)
{
	for (int i = 0; i < cell->molNum; ++i)
	{
		MethodVerlet(cell->molecules[i], dt);
		CorrectPosition(cell->molecules[i], cell->sizeX, cell->sizeY, cell->sizeZ);
	}
}

void DinamicIterate(Cell* cell, float dt)
{
	ClearForceAccel(cell);
	MakeInteractions(cell);
	RecalculateParametres(cell, dt);
}

void DinamicInit(Cell* cell, float dt)
{
	ClearForceAccel(cell);
	for (int i = 0; i < cell->molNum; ++i)
	{
		MethodEuler(cell->molecules[i], dt);
		CorrectPosition(cell->molecules[i], cell->sizeX, cell->sizeY, cell->sizeZ);
	}
}