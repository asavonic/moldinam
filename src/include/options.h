
#include <getopt.h>

struct Options
{
    const char* output;
	const char* input;
    bool shortlog;
    int steps;
    float dt;
    float sigma;
    float epsilon;
    bool nolog;
};
typedef struct Options Options;

Options Options_create_default()
{
	Options opt;

    opt.output = "output.out";
    opt.input = NULL;
    opt.shortlog = false;
    opt.steps = 100000;
    opt.dt = 0.00001;
    opt.sigma = 1;
    opt.epsilon = 1;
    opt.nolog = false;
	return opt;
}

bool SetOption(Options* options, const char * const opt, const char * const value) {
    if ( strcmp(opt, "dt") == 0 ) {
        options->dt = atof(value);
        printf("dt = %f\n", options->dt);
        return true;
    }
    if ( strcmp(opt, "sigma") == 0 ) {
        options->sigma = atof(value);
        printf("sigma = %f\n", options->sigma);
        return true;
    }
    if ( strcmp(opt, "epsilon") == 0 ) {
        options->epsilon = atof(value);
        printf("epsilon = %f\n", options->epsilon);
        return true;
    }
    if ( strcmp(opt, "steps") == 0 ) {
        options->steps = atoi(value);
        printf("steps = %d\n", options->steps);
        return true;
    }
    if ( strcmp(opt, "shortlog") == 0 ) {
        options->shortlog = true;
        printf("shortlog selected\n");
        return true;
    }
    if ( strcmp(opt, "input") == 0 ) {
        options->input = value;
        printf("input = %s\n", options->input);
        return true;
    }
    if ( strcmp(opt, "output") == 0 ) {
        options->output = value;
        printf("output = %s\n", options->output);
        return true;
    }
    if ( strcmp(opt, "nolog") == 0 ) {
        options->nolog = value;
        printf("nolog selected\n");
        return true;
    }
    
    return false;
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
            {"dt",       required_argument, 0,  0 },
            {"steps",    required_argument, 0,  0 },
            {"sigma",    required_argument, 0,  0 },
            {"epsilon",  required_argument, 0,  0 },
            {"input",    required_argument, 0,  0 },
            {"output",   required_argument, 0,  0 },
            {"shortlog", no_argument,       0,  0 },
            {"nolog", no_argument,       0,  0 },
            {0,          0,                 0,  0 }
        };

       c = getopt_long(argc, argv, "",
                 long_options, &option_index);
        if (c == -1)
            break;

       switch (c) {
        case 0:
                if(! SetOption(&options, long_options[option_index].name, optarg) ) {
                    fprintf(stderr, "Error! Unexpected argument %s",  long_options[option_index].name);
                }
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
