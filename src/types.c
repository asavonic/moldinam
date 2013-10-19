#include "include/types.h"

Molecule* Molecule_random_create(float cellSizeX, float cellSizeY, float cellSizeZ, float maxSpeed)
{
	Molecule* mol = (Molecule*) malloc(sizeof(Molecule));

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
	Molecule* mol = (Molecule*) malloc(sizeof(Molecule));

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

Molecule* Molecule_empty_create()
{
	Molecule* mol = (Molecule*) malloc(sizeof(Molecule));

	mol->pos[0] = 0;
	mol->pos[1] = 0;
	mol->pos[2] = 0;

	mol->speed[0] = 0;
	mol->speed[1] = 0;
	mol->speed[2] = 0;

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





Cell* Cell_random_create(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ, float maxSpeed)
{
	Cell* cell = (Cell*) malloc( sizeof(Cell) );
	cell->molNum = molNum;
	cell->sizeX = cellSizeX;
	cell->sizeY = cellSizeY;
	cell->sizeZ = cellSizeZ;
	cell->molecules = (Molecule**) malloc(molNum * sizeof(Molecule*));

	for (int i = 0; i < molNum; ++i)
	{
		cell->molecules[i] = Molecule_random_create(cellSizeX, cellSizeY, cellSizeZ, maxSpeed);
	}

	return cell;
}


Cell* Cell_create_empty(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ)
{
	Cell* cell = (Cell*) malloc( sizeof(Cell) );
	cell->molNum = molNum;
	cell->sizeX = cellSizeX;
	cell->sizeY = cellSizeY;
	cell->sizeZ = cellSizeZ;

	cell->molecules = (Molecule**) malloc(molNum * sizeof(Molecule*));

	for (int i = 0; i < molNum; ++i)
	{
		cell->molecules[i] = Molecule_empty_create();
	}
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

Cell* Cell_ImportFromFile(FILE* file)
{
	int molNum = 0;
	float sizex;
	float sizey;
	float sizez;

	fscanf(file,"%d %f %f %f", &molNum, &sizex, &sizey, &sizez);
	printf("molecules num = %d\n", molNum);
	printf("Size X = %f\n", sizex);
	printf("Size Y = %f\n", sizey);
	printf("Size Z = %f\n", sizez);
	if (molNum <= 0)
	{
		return NULL;
	}

	Cell* cell = Cell_create_empty(molNum, sizex, sizey, sizez);


	for (int i = 0; i < molNum; ++i)
	{
		float x = 0;
		float y = 0;
		float z = 0;
		float vx = 0;
		float vy = 0;
		float vz = 0;
		fscanf(file,"%f %f %f %f %f %f", &x, &y, &z, &vx, &vy, &vz);
		printf("%f %f %f %f %f %f\n", x, y, z, vx, vy, vz);
		cell->molecules[i]->pos[0] = x;
		cell->molecules[i]->pos[1] = y;
		cell->molecules[i]->pos[2] = z;
		cell->molecules[i]->speed[0] = vx;
		cell->molecules[i]->speed[1] = vy;
		cell->molecules[i]->speed[2] = vz;

	}

	return cell;
}

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

void MoleculeSwap(Molecule* molA, Molecule* molB)
{
    Molecule tmp = *molA;
    *molA = *molB;
    *molB = tmp;
}

void CellField_Move(CellField* field, Molecule* mol, cell_id dst, cell_id src)
{
    for (int cur = src; cur != dst; ) {
        if (src > dst) {
            MoleculeSwap(field->cells[cur], mol);
            mol = field->cells[cur];
            field->cells[cur]++;
            cur--;
        } else {
            MoleculeSwap(field->cells[cur+1]-1, mol);
            mol = field->cells[cur+1]-1;
            field->cells[cur]--;
            cur++;
        }
    }
}


CellField* CellField_create_empty(int molNum, float cellSizeX, float cellSizeY, float cellSizeZ)
{
    CellField* field = (CellField*) malloc (sizeof(CellField));
    field->molecules = (Molecule*) malloc (sizeof(Molecule) * molNum);


}
