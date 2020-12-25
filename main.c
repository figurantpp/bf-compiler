#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "compilation/compilation.h"

#define BFC_EXIT_BAD_USAGE 2

// #define BF_SOURCE_FILE_PATH "/home/figurantpp/Desktop/programming/c/bf-c/compiler_input/mandel.bf"
// #define BF_OUTPUT_PROGRAM_PATH "/home/figurantpp/Desktop/programming/c/bf-c/cmake-build-debug/compiler-output"

#define compilation_modes(x) \
    x(STANDARD)              \
    x(ASSEMBLY_ONLY)         \

#define to_enum(macro) BFC_COMPILATION_MODE_##macro,

enum BFC_COMPILATION_MODE
{
    compilation_modes(to_enum)
};

#undef to_enum

int main(int argc, char *argv[])
{
#define show_usage() ({fprintf(stderr, "Usage: %s [-S] -i in_file -o out_file\n", argv[0]); })

    enum BFC_COMPILATION_MODE mode = BFC_COMPILATION_MODE_STANDARD;

    const char *input_file = NULL;
    const char *output_file = NULL;

    int option;


    while ((option = getopt(argc, argv, "Si:o:")) != -1)
    {
        switch (option)
        {
            case 'S':
                mode = BFC_COMPILATION_MODE_ASSEMBLY_ONLY;
                break;

            case 'o':
                output_file = optarg;
                break;

            case 'i':
                input_file = optarg;
                break;

            default:
                show_usage();
                exit(BFC_EXIT_BAD_USAGE);
        }
    }

    int error = 0;

    if (output_file == NULL)
    {
        fputs("Missing required output file (-o) argument.", stderr);
        error = 1;
    }

    if (input_file == NULL)
    {
        fputs("Missing required input file (-i) argument.", stderr);
        error = 1;
    }

    if (error)
    {
        show_usage();
        exit(BFC_EXIT_BAD_USAGE);
    }

    BFCCompilerFunction* functions[] = {
            [BFC_COMPILATION_MODE_STANDARD] = bfc_compile_program,
            [BFC_COMPILATION_MODE_ASSEMBLY_ONLY] = bfc_compile_assembly
    };

    return functions[mode](input_file, output_file);
}
