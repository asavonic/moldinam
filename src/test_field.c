

#include "include\types.h"


int main()
{
    
    CellField field;
    field.molecules = (Molecule*) malloc (sizeof(Molecule) * 7);

    for (int i = 0; i < 7; i++)
    {
        field.molecules[i].pos[0] = i ;
    }

    field.cells = (Molecule**) malloc (sizeof(Molecule*)*3);
    field.cells[0] = field.molecules;
    field.cells[1] = field.molecules+2;
    field.cells[2] = field.molecules+4;


    for (int i = 0; i < 7; i++)
    {
        printf("%f ",field.molecules[i].pos[0]);
    }
    printf("\n");

    CellField_Move(&field, field.molecules + 6, 0,2);

    for (int i = 0; i < 7; i++)
    {
        printf("%f ",field.molecules[i].pos[0]);
    }
    printf("\n");
}
