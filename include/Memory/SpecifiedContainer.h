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

        SpecifiedContainer()
        {
            m_pContainer = nullptr;
            m_fnCopy = nullptr;
            m_fnDestruct = nullptr;
        }

        SpecifiedContainer(IMemory* _pMemory, __allocator* _pAllocator) :
            Container(_pMemory, _pAllocator)
        {
            m_pContainer = nullptr;
            m_fnCopy = nullptr;
            m_fnDestruct = nullptr;
        }

        SpecifiedContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pSpecifiedContainer) :
            Container(_pMemory, _pAllocator),
            m_pContainer(_pSpecifiedContainer)
        {
            m_fnCopy = nullptr;
            m_fnDestruct = nullptr;
        }

        SpecifiedContainer(IMemory* _pMemory, __allocator* _pAllocator, Type* _pContainer, const copy_callback& _fnCopy, const destruct_callback& _fnDestruct) :
            Container(_pMemory, _pAllocator),
            m_pContainer(_pContainer),
            m_fnCopy(_fnCopy),
            m_fnDestruct(_fnDestruct)
        {

        }

        // Here we do not want to have any copy constructor or operator

        copy_callback m_fnCopy;
        destruct_callback m_fnDestruct;

        Type* m_pContainer; // Like fixed_vector_allocator, fixed_stack_allocator, fixed_queue_allocator, etc.

        static destruct_callback GetDestructFunction(const SpecifiedContainer* _pSpecifiedContainer)
        {
            return _pSpecifiedContainer->m_fnDestruct;
        }

        Type* GetAllocator() { return m_pContainer; }
    };

}}