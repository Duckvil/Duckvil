#pragma once

#include "Memory/SpecifiedContainer.h"

namespace Duckvil { namespace Memory {

    template <typename ContainerType, typename Type>
    struct SpecifiedResizableContainer : public SpecifiedContainer<ContainerType, Type>
    {
        typedef void (*resize_callback)(IMemory* _pMemoryInterface, SpecifiedResizableContainer* _pThis, std::size_t _ullNewSize);
        typedef void (*erase_callback)(IMemory* _pMemoryInterface, SpecifiedResizableContainer* _pThis, uint32_t _uiIndex);

        SpecifiedResizableContainer()
        {
            m_fnResize = nullptr;
            m_fnErase = nullptr;
        }

        SpecifiedResizableContainer(IMemory* _pMemory, __allocator* _pAllocator) :
            SpecifiedContainer<ContainerType, Type>(_pMemory, _pAllocator)
        {
            m_fnResize = nullptr;
            m_fnErase = nullptr;
        }

        SpecifiedResizableContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pSpecifiedContainer) :
            SpecifiedContainer<ContainerType, Type>(_pMemory, _pAllocator, _pSpecifiedContainer)
        {
            m_fnResize = nullptr;
            m_fnErase = nullptr;
        }

        SpecifiedResizableContainer(
            IMemory* _pMemory,
            __allocator* _pAllocator,
            Type* _pContainer,
            const typename SpecifiedContainer<ContainerType, Type>::copy_callback& _fnCopy,
            const typename SpecifiedContainer<ContainerType, Type>::destruct_callback& _fnDestruct,
            const resize_callback& _fnResize,
            const erase_callback& _fnErase
        ) :
            SpecifiedContainer<ContainerType, Type>(_pMemory, _pAllocator, _pContainer, _fnCopy, _fnDestruct),
            m_fnResize(_fnResize),
            m_fnErase(_fnErase)
        {

        }

        resize_callback m_fnResize;
        erase_callback m_fnErase;
    };

}}