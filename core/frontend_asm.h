//
// Created by figurantpp on 26/12/2020.
//

#ifndef BFC_FRONTEND_ASM_H
#define BFC_FRONTEND_ASM_H

#include "temp_config.h"

#define DATA_BUFFER_SIZE "8192"

#if IS_NASM

static inline void bfc_asm_increase_rsi_content(FILE *file)
{ fputs("inc byte [rsi]\n", file); }

static inline void bfc_asm_add_rsi_content(FILE *file, size_t count)
{ fprintf(file, "add byte [rsi], %zu\n", count); }

static inline void bfc_asm_decrease_rsi_content(FILE *file)
{ fputs("dec byte [rsi]\n", file); }

static inline void bfc_asm_subtract_rsi_content(FILE *file, size_t count)
{ fprintf(file, "sub byte [rsi], %zu\n", count); }

static inline void bfc_asm_increase_rsi(FILE *file)
{ fprintf(file, "inc rsi\n"); }

static inline void bfc_asm_add_rsi(FILE *file, size_t count)
{ fprintf(file, "add rsi, %zu\n", count); }

static inline void bfc_asm_decrease_rsi(FILE *file)
{ fputs("dec rsi\n", file); }

static inline void bfc_asm_subtract_rsi(FILE *file, size_t count)
{ fprintf(file, "sub rsi, %zu\n", count); }


static inline void bfc_asm_write(FILE *file)
{
    fprintf(file,
            "mov rax, 1\n"
            "syscall\n");
}

static inline void bfc_asm_read(FILE *file)
{
    fprintf(file,
            "mov rax, 0\n"
            "syscall\n");
}


static inline void bfc_asm_jump_if_zero(FILE *file, const char *location)
{

    fprintf(
            file,
            ""
            "cmp byte [rsi], 0\n"
            "je %s\n",
            location
    );

}

static inline void bfc_asm_jump_if_not_zero(FILE *file, const char *location)
{

    fprintf(
            file,
            ""
            "cmp byte [rsi], 0\n"
            "jne %s\n",
            location
    );
}


static inline void bfc_asm_set_label(FILE *file, const char *label_name)
{
    fprintf(file, "%s:\n", label_name);
}

static inline void bfc_asm_program_start(FILE *file)
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

static inline void bfc_asm_program_end(FILE *file)
{
    fputs(
            ""
            "mov rax, 60\n"
            "mov rdi, 0\n"
            "syscall\n",
            file
    );
}


#else

static inline void bfc_asm_increase_rsi_content(FILE *file)
{ fputs("incb (%rsi)\n", file); }

static inline void bfc_asm_add_rsi_content(FILE *file, size_t count)
{ fprintf(file, "addb $%zu, (%%rsi)\n", count); }

static inline void bfc_asm_decrease_rsi_content(FILE *file)
{ fputs("decb (%rsi)\n", file); }

static inline void bfc_asm_subtract_rsi_content(FILE *file, size_t count)
{ fprintf(file, "subb $%zu, (%%rsi)\n", count); }

static inline void bfc_asm_increase_rsi(FILE *file)
{ fprintf(file, "inc %%rsi\n"); }

static inline void bfc_asm_add_rsi(FILE *file, size_t count)
{ fprintf(file, "add $%zu, %%rsi\n", count); }

static inline void bfc_asm_decrease_rsi(FILE *file)
{ fputs("dec %rsi\n", file); }

static inline void bfc_asm_subtract_rsi(FILE *file, size_t count)
{ fprintf(file, "sub $%zu, %%rsi\n", count); }


static inline void bfc_asm_write(FILE *file)
{
    fprintf(file,
            "mov $1, %%rax\n"
            "syscall\n");
}

static inline void bfc_asm_read(FILE *file)
{
    fprintf(file,
            "mov $0, %%rax\n"
            "syscall\n");
}


static inline void bfc_asm_jump_if_zero(FILE *file, const char *location)
{

    fprintf(
            file,
            ""
            "cmpb $0, (%%rsi)\n"
            "je %s\n",
            location
    );

}

static inline void bfc_asm_jump_if_not_zero(FILE *file, const char *location)
{

    fprintf(
            file,
            ""
            "cmpb $0, (%%rsi)\n"
            "jne %s\n",
            location
    );
}


static inline void bfc_asm_set_label(FILE *file, const char *label_name)
{
    fprintf(file, "%s:\n", label_name);
}

static inline void bfc_asm_program_start(FILE *file)
{
    fputs(
            ""
            ".bss\n"
            ".global _start\n"
            ".lcomm buffer, " DATA_BUFFER_SIZE "\n"
            ".text\n"
            "_start:\n"
            "mov $1, %rdi\n"
            "mov $1, %rdx\n"
            "mov $buffer, %rsi\n",
            file
    );
}

static inline void bfc_asm_program_end(FILE *file)
{
    fputs(
            ""
            "mov $60, %rax\n"
            "mov $0, %rdi\n"
            "syscall\n",
            file
    );
}


#endif

#endif //BFC_FRONTEND_ASM_H
