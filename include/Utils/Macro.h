#pragma once

#define DUCKVIL_ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define DUCKVIL_CAT(a, b) a ## b
#define DUCKVIL_EXPORT extern "C" __declspec(dllexport)