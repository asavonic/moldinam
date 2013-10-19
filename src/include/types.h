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

Molecule* Molecule_random_create(float cellSizeX, float cellSizeY, float cellSizeZ, float maxSpeed);
Molecule* Molecule_create(float pos[3], float speed[3]);
Molecule* Molecule_empty_create();
void Molecule_free(Molecule* mol);


struct objCell
{
	Molecule** molecules;
	int molNum;
	float sizeX;
	float sizeY;
	float sizeZ;
};

typedef struct objCell Cell;
Cell* Cell_random_create(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ, float maxSpeed);
Cell* Cell_create_empty(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ);
Cell* Cell_ImportFromFile(FILE* file);
void PrintCellCondition(FILE* file, Cell* cell);
void Cell_free(Cell* cell);

typedef unsigned int cell_id ;

struct objCellField
{
	Molecule* molecules;
    Molecule** cells;
};

typedef struct objCellField CellField;
void CellField_Move(CellField* field, Molecule* mol, cell_id dst, cell_id src);
CellField* CellField_random_create(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ, float maxSpeed);
CellField* CellField_create_empty(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ);
CellField* CellField_ImportFromFile(FILE* file);
void PrintCellFieldCondition(FILE* file, CellField* cell);
void CellField_free(CellField* cell);

#endif
