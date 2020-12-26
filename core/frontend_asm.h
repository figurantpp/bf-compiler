//
// Created by figurantpp on 26/12/2020.
//

#ifndef BFC_FRONTEND_ASM_H
#define BFC_FRONTEND_ASM_H

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

#endif //BFC_FRONTEND_ASM_H
