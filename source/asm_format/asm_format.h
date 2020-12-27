//
// Created by figurantpp on 26/12/2020.
//

#ifndef BFC_ASM_FORMAT_H
#define BFC_ASM_FORMAT_H

#include <stdio.h>

typedef void AssemblyOutputFormatCallback(FILE *file);
typedef void AssemblyOutputFormatIntegerCallback(FILE *file, size_t value);
typedef void AssemblyOutputFormatStringCallback(FILE *file, const char * value);

struct AssemblerInfo;

typedef int AssemblerExecutor(const char *input_file_name, const char *object_file_name);

struct AssemblerInfo
{
    const char *assembler_name;
    const char *assembler_path;
    AssemblerExecutor *exec;
};

struct AssemblyOutputFormat
{
    AssemblyOutputFormatCallback *increase_rsi_content;
    AssemblyOutputFormatIntegerCallback *add_rsi_content;

    AssemblyOutputFormatCallback *decrease_rsi_content;
    AssemblyOutputFormatIntegerCallback *subtract_rsi_content;

    AssemblyOutputFormatCallback *increase_rsi;
    AssemblyOutputFormatIntegerCallback *add_rsi;

    AssemblyOutputFormatCallback *decrease_rsi;
    AssemblyOutputFormatIntegerCallback *subtract_rsi;

    AssemblyOutputFormatCallback *write_syscall;
    AssemblyOutputFormatCallback *read_syscall;

    AssemblyOutputFormatStringCallback *jump_if_zero;
    AssemblyOutputFormatStringCallback *jump_if_not_zero;
    AssemblyOutputFormatStringCallback *set_label;

    AssemblyOutputFormatCallback *program_start;
    AssemblyOutputFormatCallback *program_end;

};

struct CompilationFormat
{
    struct AssemblyOutputFormat output_format;
    struct AssemblerInfo assembler_info;
};

const struct CompilationFormat *bfc_yasm_assembly_format();
const struct CompilationFormat *bfc_gnu_assembly_format();

#endif //BFC_ASM_FORMAT_H
