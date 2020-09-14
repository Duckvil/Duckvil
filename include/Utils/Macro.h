#pragma once

#define DUCKVIL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define DUCKVIL_CAT(a, b) a ## b

#ifdef DUCKVIL_PLATFORM_WINDOWS
#define DUCKVIL_EXPORT extern "C" __declspec(dllexport)
#else
#ifdef DUCKVIL_PLATFORM_LINUX
#define DUCKVIL_EXPORT extern "C"
#endif
#endif