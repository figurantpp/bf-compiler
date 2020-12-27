//
// Created by figurantpp on 26/12/2020.
//

#ifndef BFC_USER_INTERFACE_STATIC_H
#define BFC_USER_INTERFACE_STATIC_H


/* Exit Codes */

#define BFC_EXIT_BAD_USAGE 50

// Enums

// Compiler output option, such as -S

enum CompilationMode
{
    BFC_COMPILATION_MODE_STANDARD,
    BFC_COMPILATION_MODE_ASSEMBLY_ONLY
};

// Compiler output syntax and assembler info

enum CompilationType
{
    BFC_COMPILATION_TYPE_YASM,
    BFC_COMPILATION_TYPE_GNU,
};

enum CompilationType get_assembly_format(const char *given)
{
    struct
    {
        const char *name;
        enum CompilationType type;
    } compilation_type_names[] = {

    };

    for (size_t i = 0; i < sizeof(compilation_type_names) / sizeof(*compilation_type_names); i++)
    {
        if (strcmp(compilation_type_names[i].name, given) == 0)
        {
            return compilation_type_names[i].type;
        }
    }

    return -1;
}

#endif //BFC_USER_INTERFACE_STATIC_H
