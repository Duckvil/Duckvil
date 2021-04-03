#include "PlugNPlay/Plugin.h"

namespace Duckvil { namespace PlugNPlay {

    void* instantiate_plugin(const __module& _module, const __module_information& _module_info)
    {
        __plugin_details _details = {};

        get_variable(_module, _module_info, "g_pluginDetails", &_details);

        return _details.m_pInstance();
    }

    void* instantiate_plugin(const __module& _module, const __module_information& _module_info, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator)
    {
        __plugin_details _details = {};

        get_variable(_module, _module_info, "g_pluginDetails", &_details);

        return _details.m_pAllocator(_pMemoryInterface, _pAllocator);
    }

    void delete_plugin_instance(const __module& _module, const __module_information& _module_info, Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, void* _pointer)
    {
        __plugin_details _details = {};

        get_variable(_module, _module_info, "g_pluginDetails", &_details);

        _details.m_pDeallocator(_pMemoryInterface, _pAllocator, _pointer);
    }

}}