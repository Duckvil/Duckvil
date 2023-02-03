#pragma once

#define DUCKVIL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define DUCKVIL_CAT(a, b) a##b
#define DUCKVIL_TO_STRING(str) #str
#define DUCKVIL_STATIC_ASSERT(exp, message) static_assert(exp, message)

#ifndef DUCKVIL_AST
#ifdef DUCKVIL_PLATFORM_WINDOWS
#define DUCKVIL_EXPORT extern "C" __declspec(dllexport)
#else
#ifdef DUCKVIL_PLATFORM_LINUX
#define DUCKVIL_EXPORT extern "C"
// TODO: GCC shared library export
#endif
#endif
#else
#define DUCKVIL_EXPORT
#endif