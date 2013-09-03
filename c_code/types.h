#ifndef __TYPES_H
#define __TYPES_H

#include <stdlib.h>
#include <stdio.h>
#if defined(_WIN32) || defined (_WIN64) /*ïåðâûé äëÿ îáåèõ îïðåäåë¸í*/
	#define drand48()	(rand()*(1./35556))
#endif
struct objMolecule
{
	float pos[3];
	float prevpos[3];
	float speed[3];
	float accel[3];
	float force[3];
} ;

typedef struct objMolecule Molecule;

Molecule* Molecule_random_create(float cellSizeX, float cellSizeY, float cellSizeZ, float maxSpeed)
{
	Molecule* mol = malloc(sizeof(Molecule));

	mol->pos[0] = rand() % (int)cellSizeX + drand48();
	mol->pos[1] = rand() % (int)cellSizeY + drand48();
	mol->pos[2] = rand() % (int)cellSizeZ + drand48();

	if (maxSpeed != 0)
	{
		mol->speed[0] = rand() % ( (int)maxSpeed * 2) + drand48() - maxSpeed ;
		mol->speed[1] = rand() % ( (int)maxSpeed * 2) + drand48() - maxSpeed ;
		mol->speed[2] = rand() % ( (int)maxSpeed * 2) + drand48() - maxSpeed ;
	}
	else
	{
		mol->speed[0] = 0;
		mol->speed[1] = 0;
		mol->speed[2] = 0;
	}

	mol->accel[0] = 0;
	mol->accel[1] = 0;
	mol->accel[2] = 0;
	mol->force[0] = 0;
	mol->force[1] = 0;
	mol->force[2] = 0;

	return mol;
}
Molecule* Molecule_create(float pos[3], float speed[3])
{
	Molecule* mol = malloc(sizeof(Molecule));

	mol->pos[0] = pos[0];
	mol->pos[1] = pos[1];
	mol->pos[2] = pos[2];

	mol->speed[0] = speed[0];
	mol->speed[1] = speed[1];
	mol->speed[2] = speed[2];

	mol->accel[0] = 0;
	mol->accel[1] = 0;
	mol->accel[2] = 0;
	mol->force[0] = 0;
	mol->force[1] = 0;
	mol->force[2] = 0;

	return mol;
}

void Molecule_free(Molecule* mol)
{
	free(mol);
}

struct objCell
{
	Molecule** molecules;
	int molNum;
	float sizeX;
	float sizeY;
	float sizeZ;
};

typedef struct objCell Cell;


// bool Compare(Molecule* first, Molecule* second)
// {
// 	//returns true when first > second
// 	if ( first->pos[0] > second->pos[0] )
// 	{
// 		return true;
// 	}

// 	if (first->pos[0] == second->pos[0] && first->pos[1] > second->pos[1])
// 	{
// 		return true;
// 	}

// 	return false;
// }
// void Sort(Molecule** molecules, int count)
// {
// 	for (int i = 0; i < count - 1; ++i)
// 	{
// 		for (int j = i+1; j < count; ++j)
// 		{
// 			if ( Compare(molecules[i],molecules[j]) )
// 			{
// 				Molecule* tmp = molecules[i];
// 				molecules[i] = molecules[j];
// 				molecules[j] = tmp;
// 			}
// 		}
// 	}
// }

Cell* Cell_random_create(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ, float maxSpeed)
{
	Cell* cell = malloc( sizeof(Cell) );
	cell->molNum = molNum;
	cell->sizeX = cellSizeX;
	cell->sizeY = cellSizeY;
	cell->sizeZ = cellSizeZ;
	cell->molecules = malloc(molNum * sizeof(Molecule*));

	for (int i = 0; i < molNum; ++i)
	{
		cell->molecules[i] = Molecule_random_create(cellSizeX, cellSizeY, cellSizeZ, maxSpeed);
	}

	return cell;
}


Cell* Cell_create_empty(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ)
{
	Cell* cell = malloc( sizeof(Cell) );
	cell->molNum = molNum;
	cell->sizeX = cellSizeX;
	cell->sizeY = cellSizeY;
	cell->sizeZ = cellSizeZ;

	cell->molecules = malloc(molNum * sizeof(Molecule*));

	return cell;
}


void Cell_free(Cell* cell)
{
	for (int i = 0; i < cell->molNum; ++i)
	{
		Molecule_free(cell->molecules[i]);
	}
	free(cell->molecules);
	free(cell);
}

// struct objCellField
// {
// 	Cell** cells;
// 	int cellNum;
// 	float sizeX;
// 	float sizeY;
// };
// typedef struct objCellField CellField;

// CellField* CellField_create(int cellnum, Cell* initCell)
// {
// 	CellField* cellField= malloc (sizeof(CellField));
// 	cellField->cellNum = cellnum;
// 	cellField->cells = malloc (sizeof(Cell*) * cellnum);

// 	Sort(initCell->molecules, initCell->molNum);

// 	int moleculesInCell = initCell->molNum / cellnum;

// 	for (int i = 0; i < cellnum; ++i)
// 	{
// 		for (int j = i*moleculesInCell; j < (i+1) * moleculesInCell && j < initCell->molNum; ++j)
// 		{
// 			cellField
// 		} 
// 	}
// }

void PrintCellCondition(FILE* file, Cell* cell)
{
	// x y speedx speedy forcex forcey accelx accely 
	//empty string ends block
	for (int i = 0; i < cell->molNum; ++i)
	{
		fprintf(file, "%f %f %f %f %f %f %f %f %f %f %f %f\n", cell->molecules[i]->pos[0],
											 cell->molecules[i]->pos[1],
											 cell->molecules[i]->pos[2],
											 cell->molecules[i]->speed[0],
											 cell->molecules[i]->speed[1],
											 cell->molecules[i]->speed[2],
											 cell->molecules[i]->force[0],
											 cell->molecules[i]->force[1],
											 cell->molecules[i]->force[2],
											 cell->molecules[i]->accel[0],
											 cell->molecules[i]->accel[1],
											 cell->molecules[i]->accel[2]);
	}
	fprintf(file, "\n");
}

#endif