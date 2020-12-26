//
// Created by figurantpp on 26/12/2020.
//

#ifndef BFC_FRONTEND_ASM_H
#define BFC_FRONTEND_ASM_H

#define bfc_asm_increase_rsi_content(file) ({fputs("inc byte [rsi]\n", file);})
#define bfc_asm_add_rsi_content(file, count) ({fprintf(file, "add byte [rsi], %zu\n", count);})

#define bfc_asm_decrease_rsi_content(file) ({fputs("dec byte [rsi]\n", file);})
#define bfc_asm_subtract_rsi_content(file, count) ({fprintf(file, "sub byte [rsi], %zu\n", count);})

#define bfc_asm_increase_rsi(file) ({fprintf(file, "inc rsi\n");})
#define bfc_asm_add_rsi(file, count) ({fprintf(file, "add rsi, %zu\n", count);})

#define bfc_asm_decrease_rsi(file) ({fputs("dec rsi\n", file);})
#define bfc_asm_subtract_rsi(file, count) ({fprintf(file, "sub rsi, %zu\n", count);})


#define bfc_asm_write(file) \
    ({                      \
        fprintf(file, \
            "mov rax, 1\n" \
            "syscall\n"); \
    })

#define bfc_asm_read(file) \
    ({                      \
        fprintf(file, \
            "mov rax, 0\n" \
            "syscall\n"); \
    })


#define bfc_asm_jump_if_zero(file, location) \
({                                           \
                                             \
    fprintf( \
    file, \
    "" \
    "cmp byte [rsi], 0\n" \
    "je %s\n", \
    location\
    ); \
\
})

#define bfc_asm_jump_if_not_zero(file, location) \
({                                           \
                                             \
    fprintf( \
    file, \
    "" \
    "cmp byte [rsi], 0\n" \
    "jne %s\n", \
    location\
    ); \
\
})


#define bfc_asm_set_label(file, label_name) \
({                                          \
 fprintf(file, "%s:\n", label_name);                                           \
})

#endif //BFC_FRONTEND_ASM_H
