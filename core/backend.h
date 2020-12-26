//
// Created by figurantpp on 26/12/2020.
//

#ifndef BFC_BACKEND_H
#define BFC_BACKEND_H

#include "../asm_format/asm_format.h"

int bfc_run_backend(const char *output_file_name, const char *assembly_file_name, const struct AssemblerInfo *assembler);

#endif //BFC_BACKEND_H
