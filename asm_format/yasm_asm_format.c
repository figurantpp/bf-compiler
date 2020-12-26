//
// Created by figurantpp on 26/12/2020.
//

#include "asm_format.h"

#include <unistd.h>
#include <stdio.h>

#define DATA_BUFFER_SIZE "8192"

static void increase_rsi_content(FILE *file)
{ fputs("inc byte [rsi]\n", file); }

static void add_rsi_content(FILE *file, size_t count)
{ fprintf(file, "add byte [rsi], %zu\n", count); }

static void decrease_rsi_content(FILE *file)
{ fputs("dec byte [rsi]\n", file); }

static void subtract_rsi_content(FILE *file, size_t count)
{ fprintf(file, "sub byte [rsi], %zu\n", count); }

static void increase_rsi(FILE *file)
{ fprintf(file, "inc rsi\n"); }

static void add_rsi(FILE *file, size_t count)
{ fprintf(file, "add rsi, %zu\n", count); }

static void decrease_rsi(FILE *file)
{ fputs("dec rsi\n", file); }

static void subtract_rsi(FILE *file, size_t count)
{ fprintf(file, "sub rsi, %zu\n", count); }

static void write_syscall(FILE *file)
{
    fprintf(file,
            "mov rax, 1\n"
            "syscall\n");
}

static void read_syscall(FILE *file)
{
    fprintf(file,
            "mov rax, 0\n"
            "syscall\n");
}

static void jump_if_zero(FILE *file, const char *location)
{

    fprintf(
            file,
            ""
            "cmp byte [rsi], 0\n"
            "je %s\n",
            location
    );

}

static void jump_if_not_zero(FILE *file, const char *location)
{

    fprintf(
            file,
            ""
            "cmp byte [rsi], 0\n"
            "jne %s\n",
            location
    );
}

static void set_label(FILE *file, const char *label_name)
{
    fprintf(file, "%s:\n", label_name);
}

static void program_start(FILE *file)
{
    fputs(
            ""
            "section .bss\n"
            "buffer resb " DATA_BUFFER_SIZE "\n"
                                            "section .text\n"
                                            "global _start\n"
                                            "_start:\n"
                                            "mov rdi, 1\n"
                                            "mov rdx, 1\n"
                                            "mov rsi, buffer\n",

            file
    );
}

static void program_end(FILE *file)
{
    fputs(
            ""
            "mov rax, 60\n"
            "mov rdi, 0\n"
            "syscall\n",
            file
    );
}


#define ASSEMBLER_PATH "/usr/bin/yasm"
#define ASSEMBLER_NAME "yasm"


#define construct_assembler_call(input_name, output_name) \
 "-f", "elf64", "-g", "dwarf2", \
 input_name, "-o", output_name

static int execl_call(const char *assembly_file_name, const char *object_file_name)
{
    return execl(ASSEMBLER_PATH, ASSEMBLER_NAME,
              construct_assembler_call(assembly_file_name, object_file_name), NULL);
}

const struct CompilationFormat *bfc_yasm_assembly_format()
{
    static const struct CompilationFormat format = {

            .output_format = {
                    .increase_rsi_content = increase_rsi_content,
                    .add_rsi_content = add_rsi_content,
                    .decrease_rsi_content = decrease_rsi_content,
                    .subtract_rsi_content = subtract_rsi_content,
                    .increase_rsi = increase_rsi,
                    .add_rsi = add_rsi,
                    .decrease_rsi = decrease_rsi,
                    .subtract_rsi = subtract_rsi,
                    .write_syscall = write_syscall,
                    .read_syscall = read_syscall,
                    .jump_if_zero = jump_if_zero,
                    .jump_if_not_zero = jump_if_not_zero,
                    .set_label = set_label,
                    .program_start = program_start,
                    .program_end = program_end,
            },

            .assembler_info = {
                    .exec = execl_call,
                    .assembler_name = ASSEMBLER_NAME,
                    .assembler_path = ASSEMBLER_PATH
            }
    };

    return &format;
}
