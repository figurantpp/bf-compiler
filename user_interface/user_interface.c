//
// Created by figurantpp on 26/12/2020.
//

#include "user_interface.h"
#include "user_interface_static.h"

void show_usage(const char *program_name)
{
    fprintf(stderr,
            ""
            "Usage: %s [-h] [-S] [-f FORMAT] in_file -o out_file\n",

            program_name);
}

void show_help(const char *program_name)
{
    show_usage(program_name);

    fprintf(stderr,
            "\n"
            "   FORMAT\n"
            "       The -F option defines what assembler and what assembly format bfc will use.\n"
            "       If -S is present, the assembly output of bfc will match the one accepted by the given -F format.\n\n"

            "       Format Options:\n"
            "           gnu|gas: The GNU Portable Assembler (/usr/bin/as) and the AT&T syntax will be used.\n\n"
            "           yasm: The Yasm Modular Assembler used (/usr/bin/yasm) and the Intel syntax will be used.\n\n"

            "       The default format for -F is gnu.\n\n"
    );
}

void execute_command_line_arguments(int argc, char * const argv[])
{
#define show_usage() ({show_usage(argv[0]);})
#define show_help() ({show_help(argv[0]);})

    enum CompilationMode mode = BFC_COMPILATION_MODE_STANDARD;
    enum CompilationType format_type = BFC_COMPILATION_TYPE_GNU;

    const char *given_assembly_format = NULL;

    const char *input_file = NULL;
    const char *output_file = NULL;

    int option;

    while ((option = getopt(argc, argv, "So:f:h")) != -1)
    {
        switch (option)
        {
            case 'S':mode = BFC_COMPILATION_MODE_ASSEMBLY_ONLY;
                break;

            case 'o':output_file = optarg;
                break;

            case 'f':given_assembly_format = optarg;
                break;

            case 'h':show_help();
                exit(EXIT_SUCCESS);

            default:show_usage();
                exit(BFC_EXIT_BAD_USAGE);
        }
    }

    // getopt sets optind to the first non argument of the list, or to the end of the list (NULL) if it was not found.

    input_file = argv[optind];

    int error = 0;

    if (output_file == NULL)
    {
        fputs("Missing required output file (-o) argument.\n", stderr);
        error = 1;
    }

    if (input_file == NULL)
    {
        fputs("Missing required input file argument.\n", stderr);
        error = 1;
    }

    if (given_assembly_format != NULL)
    {
        format_type = get_assembly_format(given_assembly_format);

        if (format_type == -1)
        {
            fprintf(stderr, "Invalid format option (-f) '%s'\n\n", given_assembly_format);
            error = 1;
        }
    }

    if (error)
    {
        show_usage();
        exit(BFC_EXIT_BAD_USAGE);
    }

    const struct CompilationFormat *formats[] = {
            [BFC_COMPILATION_TYPE_GNU] = bfc_gnu_assembly_format(),
            [BFC_COMPILATION_TYPE_YASM] = bfc_yasm_assembly_format(),
    };

    BFCCompilerFunction *functions[] = {
            [BFC_COMPILATION_MODE_STANDARD] = bfc_compile_program,
            [BFC_COMPILATION_MODE_ASSEMBLY_ONLY] = bfc_compile_assembly
    };

    exit(functions[mode](input_file, output_file, formats[format_type]));
}
