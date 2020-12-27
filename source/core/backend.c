//
// Created by figurantpp on 25/12/2020.
//

#include "../asm_format/asm_format.h"

#include "backend.h"

#define LINKER_PATH "/bin/ld"
#define LINKER_NAME "ld"

// This is a temporary fix. We eventually have to supply this as an argument.

// TODO: Refactor this

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BF_TEMP_OBJECT_FILE_TEMPLATE "bfc_tmp_object_file_XXXXXX"


// runs the assembler and returns the name of the object file.
char *run_assembler(const char *assembly_file_name, const struct AssemblerInfo *assembler)
{
    if (access(assembler->assembler_path, X_OK) == -1)
    {
        // File exists

        fprintf(stderr,"The selected assembler program (%s) could not be found.\n", assembler->assembler_path);

        return NULL;
    }

    char* object_file_name = strdup(BF_TEMP_OBJECT_FILE_TEMPLATE);

    if (object_file_name == NULL)
    {
        perror("Failed to allocate object file name");
        return NULL;
    }

    if (mkstemp(object_file_name) == -1)
    {
        perror("Failed to create object file");
        free(object_file_name);
        return NULL;
    }

    int child_status;

    switch (fork())
    {
        case -1:
            perror("Failed to fork child");

            remove(object_file_name);
            free(object_file_name);

            return NULL;
        case 0:
            // Child

            if (assembler->exec(assembly_file_name, object_file_name) == -1)
            {
                perror("Failed to execute assembler");
                exit(255);
            }
        default:
            break;

    }




    if (wait(&child_status) == -1 || !WIFEXITED(child_status))
    {
        perror("Failed to get child status");

        remove(object_file_name);
        free(object_file_name);

        return NULL;
    }

    if (WEXITSTATUS(child_status) != 0)
    {
        fprintf(stderr, "Assembler %s failed with status code %d", assembler->assembler_path,
                WEXITSTATUS(child_status));

        remove(object_file_name);
        free(object_file_name);

        return NULL;
    }

    return object_file_name;
}

int call_linker(const char *object_file_name, const char *output_file_name)
{
    if (access(LINKER_PATH, X_OK) == -1)
    {
        perror("This program's linker program (" LINKER_PATH ") is not present.");
        return -1;
    }

    int child_pid = fork();

    switch (child_pid)
    {
        case -1:
            perror("Failed to start linker process");
            return -1;
        case 0:

            // Child code

            if (execl(LINKER_PATH, LINKER_NAME, object_file_name, "-o", output_file_name, NULL) == -1)
            {
                perror("Failed to execute linker");
                exit(1);
            }
            break;
        default:
            break;
    }

    // Parent code

    int child_status = 0;

    if (wait(&child_status) == -1 || !WIFEXITED(child_status))
    {
        perror("Failed to fetch linker exit code");
        return -1;
    }

    if (WEXITSTATUS(child_status) != 0)
    {
        fprintf(stderr, "Linker (%s) failed with exit code %d\n", LINKER_NAME, WEXITSTATUS(child_status));
    }

    return 0;
}

int bfc_run_backend(const char *output_file_name, const char *assembly_file_name, const struct AssemblerInfo *assembler)
{
    char *object_file_name = run_assembler(assembly_file_name, assembler);

    if (object_file_name == NULL)
    {
        return -1;
    }

    int status = call_linker(object_file_name, output_file_name);

    remove(object_file_name);

    free(object_file_name);

    return status;
}
