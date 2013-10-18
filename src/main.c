#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "include/types.h"
#include "include/dinamic.h"
#include "include/options.h"
#include "include/portable_time.h"

int main(int argc, char **argv)
{
	printf("Started!\n");
	Options options = GetOptionsFromArguments(argc, argv);
	FILE* output = fopen(options.output, "w");

    Cell* cell;

    if (options.input != NULL) {
        FILE* input = fopen( options.input, "r" );
        cell = Cell_ImportFromFile(input);
    } else {
        float sizex = 10;
        float sizey = 10;
        float sizez = 10;
        float maxRandomSpeed = 3;
        int moleculesNum = 10;
        cell = Cell_random_create(moleculesNum, sizex, sizey, sizez, maxRandomSpeed);
    }

    float dt = options.dt;
    int steps = options.steps;

    if (!options.nolog) {
        PrintCellCondition(output, cell);
    }

    float starttime = PortableGetTime();

	DinamicInit(cell, dt);
    if (!options.nolog) {
        PrintCellCondition(output, cell);
    }
	for (int i = 0; i < steps; ++i)
	{
		DinamicIterate(cell, dt);
		if (i % 500== 0)
		{
            if (!options.nolog) {
                PrintCellCondition(output, cell);
            }
		}
	}
	
	Cell_free(cell);
    float finishtime = PortableGetTime();

	fclose(output);
	printf("Finished!\n");
    printf("Time = %f\n", finishtime - starttime);
	return 0;
}
