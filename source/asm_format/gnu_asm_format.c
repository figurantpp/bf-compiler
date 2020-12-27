//
// Created by figurantpp on 26/12/2020.
//

#include "asm_format.h"

#include <unistd.h>

#define ASSEMBLER_PATH "/usr/bin/as"
#define ASSEMBLER_NAME "as"

#define construct_assembler_call(input_name, output_name) \
"--gen-debug", "-mmnemonic=att", "-msyntax=att",  input_name, "-o", output_name

static int execl_call(const char *assembly_file_name, const char *object_file_name)
{
    return execl(ASSEMBLER_PATH, ASSEMBLER_NAME,
                 construct_assembler_call(assembly_file_name, object_file_name), NULL);
}

#define DATA_BUFFER_SIZE "8192"

static inline void increase_rsi_content(FILE *file)
{ fputs("incb (%rsi)\n", file); }

static inline void add_rsi_content(FILE *file, size_t count)
{ fprintf(file, "addb $%zu, (%%rsi)\n", count); }

static inline void decrease_rsi_content(FILE *file)
{ fputs("decb (%rsi)\n", file); }

static inline void subtract_rsi_content(FILE *file, size_t count)
{ fprintf(file, "subb $%zu, (%%rsi)\n", count); }

static inline void increase_rsi(FILE *file)
{ fprintf(file, "inc %%rsi\n"); }

static inline void add_rsi(FILE *file, size_t count)
{ fprintf(file, "add $%zu, %%rsi\n", count); }

static inline void decrease_rsi(FILE *file)
{ fputs("dec %rsi\n", file); }

static inline void subtract_rsi(FILE *file, size_t count)
{ fprintf(file, "sub $%zu, %%rsi\n", count); }

static inline void write_syscall(FILE *file)
{
    fprintf(file,
            "mov $1, %%rax\n"
            "syscall\n");
}

static inline void read_syscall(FILE *file)
{
    fprintf(file,
            "mov $0, %%rax\n"
            "syscall\n");
}

static inline void jump_if_zero(FILE *file, const char *location)
{

    fprintf(
            file,
            ""
            "cmpb $0, (%%rsi)\n"
            "je %s\n",
            location
    );

}

static inline void jump_if_not_zero(FILE *file, const char *location)
{

    fprintf(
            file,
            ""
            "cmpb $0, (%%rsi)\n"
            "jne %s\n",
            location
    );
}

static inline void set_label(FILE *file, const char *label_name)
{
    fprintf(file, "%s:\n", label_name);
}

static inline void program_start(FILE *file)
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

static inline void program_end(FILE *file)
{
    fputs(
            ""
            "mov $60, %rax\n"
            "mov $0, %rdi\n"
            "syscall\n",
            file
    );
}

const struct CompilationFormat *bfc_gnu_assembly_format()
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
