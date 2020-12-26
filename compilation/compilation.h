//
// Created by figurantpp on 25/12/2020.
//

#ifndef BFC_COMPILATION_H
#define BFC_COMPILATION_H

#endif //BFC_COMPILATION_H

#include "../asm_format/asm_format.h"

int bfc_compile_program(
        const char *source_code_file_path,
        const char *output_program_file_path,
        const struct CompilationFormat *compilation_format
);

int bfc_compile_assembly(
        const char *source_code_file_path,
        const char *assembly_file_path,
        const struct CompilationFormat *compilation_format
);

typedef int BFCCompilerFunction(
        const char *input,
        const char *output,
        const struct CompilationFormat *compilation_format
);
