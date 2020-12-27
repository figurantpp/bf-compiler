//
// Created by figurantpp on 25/12/2020.
//

#include "compilation.h"
#include "../external/stream.h"
#include "../core/frontend.h"
#include "../core/backend.h"

#include <stdio.h>
#include <stdlib.h>

#define BFC_EXIT_IO 10

#define BF_ASSEMBLY_TEMP_FILE_TEMPLATE "bfc_tmp_asm_XXXXXX"

int bfc_compile_program(
        const char *source_code_file_path,
        const char *output_program_file_path,
        const struct CompilationFormat *compilation_format
)
{
    FILE *source_code_stream = fopen(source_code_file_path, "r");

    if (source_code_stream == NULL)
    {
        perror("Failed to open source code file");
        exit(BFC_EXIT_IO);
    }

    size_t source_code_length = 0;
    char *source_code = NULL;

    source_code = read_whole_file(source_code_stream, &source_code_length);

    fclose(source_code_stream);

    if (source_code == NULL)
    {
        perror("Failed to load source code");
        exit(BFC_EXIT_IO);
    }

    char assembly_output_file_name[] = BF_ASSEMBLY_TEMP_FILE_TEMPLATE;

    int assembly_output_file = mkstemp(assembly_output_file_name);

    if (assembly_output_file == -1)
    {
        perror("Failed to create file to write assembly output");
        free(source_code);
        exit(BFC_EXIT_IO);
    }

    FILE *assembly_output_stream = fdopen(assembly_output_file, "w");

    if (assembly_output_stream == NULL)
    {
        perror("Failed to open stream to write assembly output");
        free(source_code);
        abort();
    }

    int status = bfc_run_frontend(source_code, source_code_length, assembly_output_stream, &compilation_format->output_format);

    fclose(assembly_output_stream);


    if (status == 0)
    {
        status = bfc_run_backend(output_program_file_path, assembly_output_file_name, &compilation_format->assembler_info);
    }

    remove(assembly_output_file_name);

    free(source_code);

    return status;
}

int bfc_compile_assembly(
        const char *source_code_file_path,
        const char *assembly_file_path,
        const struct CompilationFormat *compilation_format
)
{
    FILE *source_code_stream = fopen(source_code_file_path, "r");

    if (source_code_stream == NULL)
    {
        perror("Failed to read source code");
        abort();
    }

    size_t source_code_length = 0;
    char *source_code = NULL;

    source_code = read_whole_file(source_code_stream, &source_code_length);

    if (source_code == NULL)
    {
        perror("Failed to read source code");
        abort();
    }

    fclose(source_code_stream);

    FILE *assembly_output_stream = fopen(assembly_file_path, "w");

    if (assembly_output_stream == NULL)
    {
        perror("Failed to open stream to write assembly output");
        free(source_code);
        abort();
    }

    int status = bfc_run_frontend(source_code, source_code_length, assembly_output_stream, &compilation_format->output_format);

    fclose(assembly_output_stream);

    free(source_code);

    return status;
}

