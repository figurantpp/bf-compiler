//
// Created by figurantpp on 25/12/2020.
//

#ifndef BFC_COMPILATION_H
#define BFC_COMPILATION_H

#endif //BFC_COMPILATION_H

int bfc_compile_program(const char *source_code_file_path, const char *output_program_file_path);
int bfc_compile_assembly(const char *source_code_file_path, const char *assembly_file_path);

typedef int BFCCompilerFunction(const char *input, const char *output);
