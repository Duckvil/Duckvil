#pragma once

#include "Memory/SpecifiedContainer.h"

namespace Duckvil { namespace Memory {

    template <typename ContainerType, typename Type>
    struct SpecifiedResizableContainer : public SpecifiedContainer<ContainerType, Type>
    {
        typedef void (*resize_callback)(IMemory* _pMemoryInterface, SpecifiedResizableContainer* _pThis, std::size_t _ullNewSize);

        SpecifiedResizableContainer()
        {
            m_fnResize = nullptr;
        }

        SpecifiedResizableContainer(IMemory* _pMemory, __allocator* _pAllocator) :
            SpecifiedContainer<ContainerType, Type>(_pMemory, _pAllocator)
        {
            m_fnResize = nullptr;
        }

        SpecifiedResizableContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pSpecifiedContainer) :
            SpecifiedContainer<ContainerType, Type>(_pMemory, _pAllocator, _pSpecifiedContainer)
        {
            m_fnResize = nullptr;
        }

        SpecifiedResizableContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pContainer, const SpecifiedContainer<ContainerType, Type>::copy_callback& _fnCopy, const SpecifiedContainer<ContainerType, Type>::destruct_callback& _fnDestruct, const resize_callback& _fnResize) :
            SpecifiedContainer<ContainerType, Type>(_pMemory, _pAllocator, _pContainer, _fnCopy, _fnDestruct),
            m_fnResize(_fnResize)
        {

        }

        resize_callback m_fnResize;
    };

}}