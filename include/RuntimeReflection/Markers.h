#pragma once

#define DUCKVIL_BODY_MACRO_COMBINE_INNER(A, B, C, D) A##B##C##D
#define DUCKVIL_BODY_MACRO_COMBINE(A, B, C, D) DUCKVIL_BODY_MACRO_COMBINE_INNER(A, B, C, D)

#define DUCKVIL_CLASS(...) // IDK
#define DUCKVIL_CONSTRUCTOR(...) // Should be used for constructors
#define DUCKVIL_STRUCT(...) // Should be used for classes of structures
#define DUCKVIL_FUNCTION(...) // Should be used for functions
#define DUCKVIL_VARIABLE(...) // Should be used for variables
#define DUCKVIL_NAMESPACE(...) // Should be used for namespaces
#define DUCKVIL_ARGUMENT(...) // Should be used for arguments
#define DUCKVIL_CALLBACK(...)

#if defined DUCKVIL_COMPILE_REFLECTION_SOURCE
#define DUCKVIL_GENERATED_BODY DUCKVIL_BODY_MACRO_COMBINE(DUCKVIL_CURRENT_FILE_ID, _, __LINE__, _GENERATED_BODY)
#else
#define DUCKVIL_GENERATED_BODY
#endif