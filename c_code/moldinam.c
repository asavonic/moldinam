#include "types.h"
#include "dinamic.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>

struct Options
{
	bool full_log;
	const char* log_path;

	const char* input_path;
};
typedef struct Options Options;

Options Options_create_default()
{
	Options opt;
	opt.full_log = true;
	opt.log_path = "output.txt";
	opt.input_path = NULL;

	return opt;
}

Cell* Cell_ImportFromFile(FILE* file)
{
	int molNum = 0;
	float sizex;
	float sizey;
	float sizez;

	fscanf(file,"%d", &molNum);
	printf("molecules num = %d\n", molNum);
	fscanf(file,"%d", &sizex);
	printf("Size X = %d\n", sizex);
	fscanf(file,"%d", &sizey);
	printf("Size Y = %d\n", sizey);
	fscanf(file,"%d", &sizez);
	printf("Size Z = %d\n", sizez);
	if (molNum <= 0)
	{
		return NULL;
	}

	Cell* cell = Cell_create_empty(molNum, sizex, sizey);

	for (int i = 0; i < molNum; ++i)
	{
		float x = 0;
		float y = 0;
		float vx = 0;
		float vy = 0;
		fscanf(file,"%f %f %f %f", &x, &y, &vx, &vy);
		printf("%f %f %f %f\n", x, y, vx, vy);
		cell->molecules[i]->pos[0] = x;
		cell->molecules[i]->pos[1] = y;
		cell->molecules[i]->speed[0] = vx;
		cell->molecules[i]->speed[1] = vy;
	}
}

Options GetOptionsFromArguments(int argc, char **argv)
{
	Options options = Options_create_default();

	int c;
    int digit_optind = 0;

   while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static const struct option long_options[] = {
            {"dt",     required_argument, 0,  0 },
            {"steps",  required_argument, 0,  0 },
            {"sigma",  required_argument, 0,  0 },
            {"epsilon", required_argument, 0,  0 },
            {"input",  required_argument, 0, 0},
            {"output",    required_argument, 0,  0 },
            {0,         0,                 0,  0 }
        };

       c = getopt_long(argc, argv, "",
                 long_options, &option_index);
        if (c == -1)
            break;

       switch (c) {
        case 0:
            printf("option %s", long_options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;

       default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

   if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    return options;
}


int main(int argc, char **argv)
{

	printf("Started!\n");
	Options options = GetOptionsFromArguments(argc, argv);
	// FILE* output = fopen(options->log_path, "w");
	FILE* output = fopen("output.txt", "w");
	FILE* input = fopen("input.txt", "r");

	if (input != NULL)
	{
		printf("file input opened\n");
	}
	float sizex = 10;
	float sizey = 10;
	float sizez = 10;
	float maxRandomSpeed = 3;
	int moleculesNum = 10;
	float dt = 0.00001;

	Cell* cell = Cell_random_create(moleculesNum, sizex, sizey, sizez, maxRandomSpeed);
	// Cell* cell = Cell_ImportFromFile(input, sizex, sizey);
	// making custom cell
	// Molecule_free(cell->molecules[0]);
	// Molecule_free(cell->molecules[1]);
	// Molecule_free(cell->molecules[2]);

	// float pos[2] = {5.0 , 0.0};
	// float speed[2] = {0.0 , 0.0};
	// cell->molecules[0] = Molecule_create(pos,speed);
	// pos[0] = 4.0; pos[1] = 0.0;
	// cell->molecules[1] = Molecule_create(pos,speed);
	// pos[0] = 6.0;
	// cell->molecules[2] = Molecule_create(pos,speed);

	PrintCellCondition(output, cell);
	DinamicInit(cell, dt);
	PrintCellCondition(output, cell);
	for (int i = 0; i < 5000000; ++i)
	{
		DinamicIterate(cell, dt);
		if (i % 500== 0)
		{
			PrintCellCondition(output, cell);
		}
	}
	
	Cell_free(cell);
	fclose(output);
	printf("Finished!\n");	
	return 0;
}