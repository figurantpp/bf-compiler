#include <stdio.h>
#include <stdlib.h>

#include "bfc/bfc.h"
#include "external/stream.h"

#define BF_SOURCE_FILE_PATH "/home/figurantpp/Desktop/programming/c/bf-c/compiler_input/mandel.bf"
#define BF_ASSEMBLY_TEMP_FILE_TEMPLATE "bfc_tmp_asm_XXXXXX"
#define BF_OUTPUT_PROGRAM_PATH "/home/figurantpp/Desktop/programming/c/bf-c/cmake-build-debug/compiler-output"

int main()
{
    FILE *source_code_stream = fopen(BF_SOURCE_FILE_PATH, "r");

    if (source_code_stream == NULL)
    {
        perror("Failed to read source code");
        abort();
    }

    size_t source_code_length = 0;
    char *source_code = NULL;

    source_code = read_whole_file(source_code_stream, &source_code_length);

    fclose(source_code_stream);

    if (source_code == NULL)
    {
        perror("Failed to read source code");
        abort();
    }

    char assembly_output_file_name[] = BF_ASSEMBLY_TEMP_FILE_TEMPLATE;

    int assembly_output_file = mkstemp(assembly_output_file_name);

    if (assembly_output_file == -1)
    {
        perror("Failed to create file to write assembly output");
        abort();
    }

    FILE *assembly_output_stream = fdopen(assembly_output_file, "w");

    if (assembly_output_stream == NULL)
    {
        perror("Failed to open stream to write assembly output");
        abort();
    }

    int status = bfc_run_frontend(source_code, source_code_length, assembly_output_stream);

    fclose(assembly_output_stream);


    if (status == 0)
    {
        status = bfc_run_backend(BF_OUTPUT_PROGRAM_PATH, assembly_output_file_name);
    }

    remove(assembly_output_file_name);

    free(source_code);

    return status;
}
