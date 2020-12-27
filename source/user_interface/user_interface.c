//
// Created by figurantpp on 26/12/2020.
//

#include "user_interface.h"
#include "user_interface_static.h"

#define PROGRAM_VERSION "0.0.1"

// Declaring a macro that returns an rvalue is a good way of preventing us from
// writing to the global variable more than once.

const char * internal_program_name;

static inline const char * get_program_name()
{ return internal_program_name; }

#define global_program_name get_program_name()

static void write_usage(const char *program_name, FILE *file)
{
    fprintf(file,
            "\n"
            "Usage: %s [-h] [-v] [-S] [-f FORMAT] in_file -o out_file\n\n",
            program_name);
}

static void error_show_usage()
{
    write_usage(global_program_name, stderr);
}

void show_help()
{
    write_usage(global_program_name, stdout);

    fprintf(stdout,
            "\n"
            "   OPTIONS\n"
            "       -h\n"
            "           Display this help message\n\n"

            "       -v\n"
            "           Show program version\n\n"

            "       -S\n"
            "           Stop after compiling code.\n"
            "           The compiler will not execute the assembler nor the linker, but will only compile the source code instead.\n"
            "           The compiler's output (-o flag) will be the produced assembly code on instead of the assembled and liked program.\n"
            "           The produced assembly code will match the one given by -f\n\n"

            "   FORMAT\n"
            "       The -f option defines the assembly output format and the used assembler the compiler will use.\n"
            "       If -S is present, the assembly output of the comiler will match the one accepted by the given -F format.\n\n"

            "       Format Options:\n"
            "           gnu|gas: The GNU Portable Assembler (/usr/bin/as) and the AT&T syntax will be used.\n\n"
            "           yasm: The Yasm Modular Assembler used (/usr/bin/yasm) and the Intel syntax will be used.\n\n"

            "       The default format for -F is \"gas\".\n\n"
    );
}

void show_version()
{
    printf("%s (BFC) %s\nAuthor: figurant++\n", global_program_name, PROGRAM_VERSION);
}

void execute_command_line_arguments(int argc, char *const argv[])
{
    // yes, this can happen.
    if (argc == 0)
    {
        write_usage("bfc", stderr);
        exit(BFC_EXIT_BAD_USAGE);
    }

    internal_program_name = argv[0];

    enum CompilationMode mode = BFC_COMPILATION_MODE_STANDARD;
    enum CompilationType format_type = BFC_COMPILATION_TYPE_GNU;

    const char *given_assembly_format = NULL;

    const char *input_file = NULL;
    const char *output_file = NULL;

    int option;

    while ((option = getopt(argc, argv, "So:f:hv")) != -1)
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
            case 'v':show_version();
                exit(EXIT_SUCCESS);

            default:error_show_usage();
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
        error_show_usage();
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
