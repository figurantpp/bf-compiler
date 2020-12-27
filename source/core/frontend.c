//
// Created by figurantpp on 24/12/2020.
//

#include "frontend.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#define LEFT_BRACE_PREFIX "left_brace_"
#define RIGHT_BRACE_PREFIX "right_brace_"


#define NUMERIC_BUFFER_SIZE 64

struct BFCState
{
    const struct AssemblyOutputFormat *output_format;

    const char *source_code;
    const char *source_code_cursor;

    struct BFCLabelData
    {
        char *self_label;
        char *matching_label;
    } *label_table;

    FILE *output_file;
    size_t source_code_length;
};


#include "frontend.h"

char *bfc_get_left_label_name(size_t left_brace_index)
{
    size_t left_label_size = strlen(LEFT_BRACE_PREFIX) + NUMERIC_BUFFER_SIZE + 1;

    char *left_label = malloc(left_label_size);

    if (!left_label)
    { return NULL; }

    snprintf(left_label, left_label_size, "%s%zu", LEFT_BRACE_PREFIX, left_brace_index);

    return left_label;
}

char *bfc_get_right_label_name(size_t right_brace_index)
{
    size_t right_label_size = strlen(RIGHT_BRACE_PREFIX) + NUMERIC_BUFFER_SIZE + 1;

    char *right_label = malloc(right_label_size);

    if (!right_label)
    { return NULL; }

    snprintf(right_label, right_label_size, "%s%zu", RIGHT_BRACE_PREFIX, right_brace_index);

    return right_label;
}

void bfc_compile_data_increment(struct BFCState *state)
{
    size_t increment_count = 0;

    while (*state->source_code_cursor == '+')
    {
        increment_count++;
        state->source_code_cursor++;
    }

    if (increment_count == 1)
    {
        state->output_format->increase_rsi_content(state->output_file);
    }
    else
    {
        state->output_format->add_rsi_content(state->output_file, increment_count);
    }

    fflush(state->output_file);
}

void bfc_compile_data_decrement(struct BFCState *state)
{
    size_t decrement_count = 0;

    while (*state->source_code_cursor == '-')
    {
        decrement_count++;
        state->source_code_cursor++;
    }

    if (decrement_count == 1)
    {
        state->output_format->decrease_rsi_content(state->output_file);
    }
    else
    {
        state->output_format->subtract_rsi_content(state->output_file, decrement_count);

    }

    fflush(state->output_file);
}

void bfc_compile_data_advance(struct BFCState *state)
{
    size_t advance_count = 0;

    while (*state->source_code_cursor == '>')
    {
        advance_count++;
        state->source_code_cursor++;
    }

    if (advance_count == 1)
    {
        state->output_format->increase_rsi(state->output_file);
    }
    else
    {
        state->output_format->add_rsi(state->output_file, advance_count);
    }

    fflush(state->output_file);
}


void bfc_compile_data_retreat(struct BFCState *state)
{
    size_t retreat_count = 0;

    while (*state->source_code_cursor == '<')
    {
        retreat_count++;
        state->source_code_cursor++;
    }

    if (retreat_count == 1)
    {
        state->output_format->decrease_rsi(state->output_file);
    }
    else
    {
        state->output_format->subtract_rsi(state->output_file, retreat_count);
    }

    fflush(state->output_file);
}

void bfc_compile_write(struct BFCState *state)
{
    state->output_format->write_syscall(state->output_file);

    fflush(state->output_file);

    state->source_code_cursor++;
}

void bfc_compile_read(struct BFCState *state)
{
    state->output_format->read_syscall(state->output_file);

    fflush(state->output_file);
}

int bfc_compile_left_brace(struct BFCState *state)
{
    size_t index = state->source_code_cursor - state->source_code;

    struct BFCLabelData *label_data = state->label_table + index;

    if (label_data->self_label == NULL)
    {
        fprintf(stderr, "Invalid index %zu for self label table", index);
        return -1;
    }

    state->output_format->jump_if_zero(state->output_file, label_data->matching_label);
    state->output_format->set_label(state->output_file, label_data->self_label);

    fflush(state->output_file);

    state->source_code_cursor++;

    return 0;
}

int bfc_compile_right_brace(struct BFCState *state)
{
    size_t index = state->source_code_cursor - state->source_code;

    struct BFCLabelData *label_data = state->label_table + index;

    if (label_data->self_label == NULL)
    {
        fprintf(stderr, "Invalid index %zu for self label table", index);
        return -1;
    }

    state->output_format->jump_if_not_zero(state->output_file, label_data->matching_label);
    state->output_format->set_label(state->output_file, label_data->self_label);

    fflush(state->output_file);

    state->source_code_cursor++;

    return 0;
}

int bfc_loop(struct BFCState *state)
{
    const char *const end = state->source_code + state->source_code_length;

    bool success = 1;

    while (state->source_code_cursor != end && success)
    {
        switch (*state->source_code_cursor)
        {
            case '+':bfc_compile_data_increment(state);
                break;
            case '-':bfc_compile_data_decrement(state);
                break;
            case '>':bfc_compile_data_advance(state);
                break;
            case '<':bfc_compile_data_retreat(state);
                break;
            case '[':success = bfc_compile_left_brace(state) == 0;
                break;
            case ']':success = bfc_compile_right_brace(state) == 0;
                break;
            case '.':bfc_compile_write(state);
                break;
            case ',':bfc_compile_read(state);
                break;
            default:state->source_code_cursor++;
        }
    }

    return success ? 0 : -1;
}

void bfc_write_start(struct BFCState *state)
{
    state->output_format->program_start(state->output_file);
}

void bfc_write_end(struct BFCState *state)
{
    state->output_format->program_end(state->output_file);
}

static const char *bfc_get_matching_right_brace(struct BFCState *state, const char *const left_brace)
{
    typeof(left_brace)
            end = state->source_code + state->source_code_length;

    const char *pointer = left_brace;


    int open_count = 1;

    // We have to start looking after the current cursor
    pointer++;

    while (open_count != 0 && pointer != end)
    {
        switch (*pointer)
        {
            case ']': open_count--;
                break;
            case '[': open_count++;
                break;
        }

        pointer++;
    }

    // Pointer will point to the char after the ']' because of the while loop
    // So we need to adjust it to the preceding ']'
    pointer--;

    if (open_count != 0)
    {
        fprintf(stderr, "Unclosed bracket in char %lu",
                left_brace - state->source_code);

        return NULL;
    }

    if (*pointer != ']')
    {
        printf("Seeking failed, `*pointer` is not ']'\n");

        printf("source code start: %p\n", state->source_code);
        printf("pointer: %p\n", pointer);
        printf("end: %p\n", end);
        printf("source code length: %zu\n", state->source_code_length);
        printf("position: %lu\n", pointer - state->source_code);

        return NULL;
    }


    return pointer;
}

int bfc_setup(struct BFCState *state)
{
    const char *const end = state->source_code + state->source_code_length;

    const char *cursor = state->source_code;


    struct BFCLabelData *label_table = calloc(state->source_code_length, sizeof(struct BFCLabelData));

    if (label_table == NULL)
    {
        fputs("Failed to allocate label table", stderr);
        return -1;
    }

    while (cursor != end)
    {
        if (*cursor == '[')
        {
            const char *left_brace = cursor;
            const char *right_brace = bfc_get_matching_right_brace(state, left_brace);

            if (right_brace == NULL)
            {
                free(label_table);
                return -1;
            }

            size_t left_brace_index = left_brace - state->source_code;
            size_t right_brace_index = right_brace - state->source_code;

            char *left_label = bfc_get_left_label_name(left_brace_index);
            char *right_label = bfc_get_right_label_name(right_brace_index);


            if (left_label == NULL || right_label == NULL)
            {
                perror("Failed to allocate label for jump table\n");

                for (size_t i = 0; i < state->source_code_length; i++)
                {
                    free(label_table[i].self_label);
                    free(label_table[i].matching_label);
                }

                free(label_table);

                return -1;
            }

            label_table[left_brace_index].self_label = left_label;
            label_table[left_brace_index].matching_label = right_label;

            label_table[right_brace_index].self_label = right_label;
            label_table[right_brace_index].matching_label = left_label;
        }

        cursor++;
    }

    state->label_table = label_table;

    return 0;
}

void bfc_cleanup(struct BFCState *state)
{
    if (state->label_table)
    {
        for (size_t i = 0; i < state->source_code_length; i++)
        {
            free(state->label_table[i].self_label);

            state->label_table[i].self_label = NULL;
        }

        free(state->label_table);
    }
}


#define null_check(parameter, return_value) \
    ({                        \
        if (parameter == NULL)\
        {                     \
            fprintf(stderr, "%s: NULL parameter %s given. Aborting.\n", __FUNCTION__, #parameter); \
            errno = EINVAL; \
            return return_value; \
        }                 \
    })

int bfc_run_frontend(const char *source_code, size_t source_code_length, FILE *output_file,
                     const struct AssemblyOutputFormat *output_format)
{
    null_check(output_file, -1);
    null_check(output_format, -1);

    struct BFCState state[1] = {};

    state->output_file = output_file;
    state->source_code_length = source_code_length;
    state->source_code = source_code;
    state->source_code_cursor = source_code;
    state->output_format = output_format;

    bfc_write_start(state);

    if (bfc_setup(state) == -1)
    {
        return -1;
    }

    if (bfc_loop(state) == -1)
    {
        bfc_cleanup(state);
        return -1;
    }

    fflush(state->output_file);

    bfc_cleanup(state);

    bfc_write_end(state);

    return 0;
}

