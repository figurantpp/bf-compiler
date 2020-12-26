#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "compilation/compilation.h"

#define BFC_EXIT_BAD_USAGE 2

// #define BF_SOURCE_FILE_PATH "/home/figurantpp/Desktop/programming/c/bf-c/compiler_input/mandel.bf"
// #define BF_OUTPUT_PROGRAM_PATH "/home/figurantpp/Desktop/programming/c/bf-c/cmake-build-debug/compiler-output"

#define compilation_modes(x) \
    x(STANDARD)              \
    x(ASSEMBLY_ONLY)         \

#define to_enum(macro) BFC_COMPILATION_MODE_##macro,

enum CompilationMode
{
    compilation_modes(to_enum)
};

#undef to_enum

#define COMMA ,

#define assembly_formats(x) \
    x(YASM, {"yasm"})                 \
    x(GNU, {"gnu" COMMA "gas"})

#define to_enum(format, names) BFC_COMPILATION_FORMAT_##format,

enum CompilationFormatType
{
    assembly_formats(to_enum)
};

#undef to_enum

#define to_array(format, names) const char *format##_names[] = names;
#define to_loop(format, names) \
for (size_t i = 0; i < sizeof(format##_names) / sizeof(char*); i++) \
{                                                                                                  \
    if (strcmp(given, format##_names[i]) == 0)                      \
    {                          \
        return BFC_COMPILATION_FORMAT_##format;                       \
    }\
}

static enum CompilationFormatType get_assembly_format(const char *given)
{
    assembly_formats(to_array)
    assembly_formats(to_loop)

    return -1;
}

void show_usage(const char *program_name)
{
    fprintf(stderr,
            ""
            "Usage: %s [-S] [-f FORMAT] -i in_file -o out_file\n\n"
            "   FORMAT\n"
            "       The -F option defines what assembler and what assembly format bfc will use.\n"
            "       If -S is present, the assembly output of bfc will match the one accepted by the given -F format.\n\n"

            "       Format Options:\n"
            "           gnu|gas: The GNU Portable Assembler (/usr/bin/as) and the AT&T syntax will be used.\n\n"
            "           yasm: The Yasm Modular Assembler used (/usr/bin/yasm) and the Intel syntax will be used.\n\n"

            "       The default format for -F is gnu.\n\n",

            program_name);
}


int main(int argc, char *argv[])
{
#define show_usage() ({show_usage(argv[0]);})

    enum CompilationMode mode = BFC_COMPILATION_MODE_STANDARD;
    enum CompilationFormatType format_type = BFC_COMPILATION_FORMAT_GNU;

    const char *given_assembly_format = NULL;

    const char *input_file = NULL;
    const char *output_file = NULL;

    int option;


    while ((option = getopt(argc, argv, "Si:o:f:")) != -1)
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

            case 'f':
                given_assembly_format = optarg;
                break;

            default:
                show_usage();
                exit(BFC_EXIT_BAD_USAGE);
        }
    }

    int error = 0;

    if (output_file == NULL)
    {
        fputs("Missing required output file (-o) argument.\n", stderr);
        error = 1;
    }

    if (input_file == NULL)
    {
        fputs("Missing required input file (-i) argument.\n", stderr);
        error = 1;
    }

    if (given_assembly_format != NULL)
    {
        format_type = get_assembly_format(given_assembly_format);

        if (format_type == -1)
        {
            fprintf(stderr, "Invalid format option '%s'\n\n", given_assembly_format);
            error = 1;
        }
    }

    if (error)
    {
        show_usage();
        exit(BFC_EXIT_BAD_USAGE);
    }

    const struct CompilationFormat* formats[] = {
        [BFC_COMPILATION_FORMAT_GNU] = bfc_gnu_assembly_format(),
        [BFC_COMPILATION_FORMAT_YASM] = bfc_yasm_assembly_format(),
    };

    BFCCompilerFunction* functions[] = {
            [BFC_COMPILATION_MODE_STANDARD] = bfc_compile_program,
            [BFC_COMPILATION_MODE_ASSEMBLY_ONLY] = bfc_compile_assembly
    };

    return functions[mode](input_file, output_file, formats[format_type]);
}
