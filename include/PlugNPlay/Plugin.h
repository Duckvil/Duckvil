#pragma once

#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"

#include "Memory/Memory.h"

namespace Duckvil { namespace PlugNPlay {

    typedef void* (*allocator_callback)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator);
    typedef void (*free_callback)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, void* _pointer);
    typedef void* (*instance_callback)();

    enum __instantiation : uint8_t
    {
        SINGLE,
        MANY,
        NONE
    };

    struct __plugin_details
    {
        const char*         m_sPluginName;
        allocator_callback  m_pAllocator;
        instance_callback   m_pInstance;
        free_callback       m_pDeallocator;
        __instantiation     m_instantiation;
    };

    void* instantiate_plugin(const __module& _module, const __module_information& _module_info);
    void* instantiate_plugin(const __module& _module, const __module_information& _module_info, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator);

    void delete_plugin_instance(const __module& _module, const __module_information& _module_info, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, void* _pointer);

}}

#define DUCKVIL_PLUGIN(name) \
    void* construct(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator) \
    { \
        return _pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, 0, sizeof(name), alignof(name)); \
    } \
    void* construct_instance() \
    { \
        static name _instance; \
        return &_instance; \
    } \
    void free(Duckvil::Memory::ftable* _pMemoryInterface, Duckvil::Memory::free_list_allocator* _pAllocator, void* _pointer) \
    { \
        _pMemoryInterface->m_fnFreeListFree_(_pAllocator, _pointer); \
    } \
    DUCKVIL_EXPORT Duckvil::PlugNPlay::__plugin_details g_pluginDetails = \
    { \
        #name, \
        Duckvil::PlugNPlay::__instantiation::MANY, \
        &construct, \
        0, \
        &free \
    };

#define DUCKVIL_GLOBAL_PLUGIN(name) \
    void* construct_instance() \
    { \
        static name _instance; \
        return &_instance; \
    } \
    DUCKVIL_EXPORT Duckvil::PlugNPlay::__plugin_details g_pluginDetails = \
    { \
        #name, \
        Duckvil::PlugNPlay::__instantiation::SINGLE, \
        0, \
        &construct_instance, \
        0 \
    };
