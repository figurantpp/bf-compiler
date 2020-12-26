//
// Created by figurantpp on 24/12/2020.
//

#ifndef BFC_FRONTEND_H
#define BFC_FRONTEND_H

#include <stdio.h>

#include "../asm_format/asm_format.h"

/**
 * Compiles \p source_code and saves the output in \p output_file
 * @param source_code The source code to compile.
 * @param output_file The file in which to output the data.
 * @param output_format The assembly output format to use.
 * @note \p output_file Does not need to be a seekable stream.
 * @returns 0 if compilation succeeds, and -1 if it fails.
 */
int bfc_run_frontend(
        const char *source_code,
        size_t source_code_length,
        FILE *output_file,
        const struct AssemblyOutputFormat *output_format
);


#endif //BFC_FRONTEND_H
