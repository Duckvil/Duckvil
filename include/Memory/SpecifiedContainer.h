#pragma once

#include "Memory/Container.h"

namespace Duckvil { namespace Memory {

    template <typename ContainerType, typename Type>
    struct SpecifiedContainer : public Container
    {
        using container = ContainerType;
        using type = Type;

        typedef void (*copy_callback)(IMemory* _pMemoryInterface, const SpecifiedContainer& _vector, SpecifiedContainer* _pThis);
        typedef void (*destruct_callback)(IMemory* _pMemoryInterface, __allocator*, SpecifiedContainer* _pThis);
        typedef void (*resize_callback)(IMemory* _pMemoryInterface, SpecifiedContainer* _pThis, std::size_t _ullNewSize); 

        SpecifiedContainer()
        {
            m_pContainer = nullptr;
        }

        SpecifiedContainer(IMemory* _pMemory, __allocator* _pAllocator) :
            Container(_pMemory, _pAllocator)
        {

        }

        SpecifiedContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pSpecifiedContainer) :
            Container(_pMemory, _pAllocator),
            m_pContainer(_pSpecifiedContainer)
        {

        }

        SpecifiedContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pContainer, const copy_callback& _fnCopy, const destruct_callback& _fnDestruct, const resize_callback& _fnResize) :
            Container(_pMemory, _pAllocator),
            m_pContainer(_pContainer),
            m_fnCopy(_fnCopy),
            m_fnDestruct(_fnDestruct),
            m_fnResize(_fnResize)
        {

        }

        // Here we do not want to have any copy constructor or operator

        copy_callback m_fnCopy;
        destruct_callback m_fnDestruct;
        resize_callback m_fnResize;

        Type* m_pContainer; // Like fixed_vector_allocator, fixed_stack_allocator, fixed_queue_allocator, etc.
    };

}}