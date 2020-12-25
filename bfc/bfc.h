//
// Created by figurantpp on 24/12/2020.
//

#ifndef BFC_BFC_H
#define BFC_BFC_H

#include <bits/types/FILE.h>
#include <sys/wait.h>

#define BFC_DEFAULT_EXECUTION_BUFFER_LIMIT 8192lu

/**
 * Compiles \p source_code and saves the output int output_file
 * @param source_code The source code to compile.
 * @param output_file The file in which to output the data.
 * @note \p output_file Does not need to be a seekable stream.
 * @returns 0 if compilation succeeds, and -1 if it fails.
 */
int bfc_run_frontend(const char *source_code, size_t source_code_length, FILE *output_file);

int bfc_run_backend(const char *output_file_name, const char *assembly_file_name);




#endif //BFC_BFC_H
