#pragma once

#define DUCKVIL_BODY_MACRO_COMBINE_INNER(A, B, C, D) A##B##C##D
#define DUCKVIL_BODY_MACRO_COMBINE(A, B, C, D) DUCKVIL_BODY_MACRO_COMBINE_INNER(A, B, C, D)

#define DUCKVIL_STRUCTURE_MACRO_COMBINE_INNER(str, A, ...) str [[using duckvil: meta(__VA_ARGS__)]] __duckvil_ ## str ## _ ## A ## __;
#define DUCKVIL_STRUCTURE_MACRO_COMBINE(str, A, ...) DUCKVIL_STRUCTURE_MACRO_COMBINE_INNER(str, A, __VA_ARGS__)

#ifdef DUCKVIL_AST
#define DUCKVIL_CLASS(...) DUCKVIL_STRUCTURE_MACRO_COMBINE(class, __LINE__, __VA_ARGS__)
#define DUCKVIL_CONSTRUCTOR(...) [[using duckvil: meta(__VA_ARGS__)]]
#define DUCKVIL_STRUCT(...) DUCKVIL_STRUCTURE_MACRO_COMBINE(struct, __LINE__, __VA_ARGS__)
#define DUCKVIL_FUNCTION(...) [[using duckvil: meta(__VA_ARGS__)]]
#define DUCKVIL_VARIABLE(...) [[using duckvil: meta(__VA_ARGS__)]]
#define DUCKVIL_NAMESPACE(...) [[using duckvil: meta(__VA_ARGS__)]]
#define DUCKVIL_ARGUMENT(...) [[using duckvil: meta(__VA_ARGS__)]]
#define DUCKVIL_CALLBACK(...) [[using duckvil: meta(__VA_ARGS__)]]
#else
#define DUCKVIL_CLASS(...) // IDK
#define DUCKVIL_CONSTRUCTOR(...) // Should be used for constructors
#define DUCKVIL_STRUCT(...) // Should be used for classes of structures
#define DUCKVIL_FUNCTION(...) // Should be used for functions
#define DUCKVIL_VARIABLE(...) // Should be used for variables
#define DUCKVIL_NAMESPACE(...) // Should be used for namespaces
#define DUCKVIL_ARGUMENT(...) // Should be used for arguments
#define DUCKVIL_CALLBACK(...)
#endif

#ifdef DUCKVIL_AST
#define DUCKVIL_GENERATED_BODY class __duckvil_generated_body__;
#else
#if defined DUCKVIL_COMPILE_REFLECTION_SOURCE
#define DUCKVIL_GENERATED_BODY DUCKVIL_BODY_MACRO_COMBINE(DUCKVIL_CURRENT_FILE_ID, _, __LINE__, _GENERATED_BODY)
#else
#define DUCKVIL_GENERATED_BODY 
#endif
#endif