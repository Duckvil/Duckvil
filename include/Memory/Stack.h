#pragma once

#include "Memory/FixedStackAllocator.h"
#include "Memory/Iterator.h"
#include "Memory/SpecifiedContainer.h"
#include "Memory/FreeListAllocator.h"

#include <type_traits>
#include <utility>

#include "tracy/Tracy.hpp"

namespace Duckvil { namespace Memory {

    template <typename Type>
    class Stack : public SpecifiedContainer<Type, fixed_stack_allocator>
    {
    public:
        typedef Memory::Iterator<Type> Iterator;
        typedef Memory::Iterator<const Type> ConstIterator;

        using SContainer = SpecifiedContainer<Type, fixed_stack_allocator>;

    private:
        // fixed_stack_allocator* m_pContainer;
        // ftable* m_pMemoryInterface;
        // allocator* m_pAllocator;

        static void free_list_copy(const SContainer& _specifiedContainer, SContainer* _pThis)
        {
            const Stack& _stack = (const Stack&)_specifiedContainer;

            _pThis->m_pContainer = (fixed_stack_allocator*)free_list_allocate(_pThis->m_pMemoryInterface, static_cast<free_list_allocator*>(_stack.m_pAllocator), sizeof(fixed_stack_allocator) + (sizeof(Type) * _stack.GetSize()), alignof(fixed_stack_allocator));

            TracyMessageL("Copy stack");

            static_cast<fixed_stack_allocator*>(_pThis->m_pContainer)->m_ullBlockSize = static_cast<fixed_stack_allocator*>(_stack.m_pContainer)->m_ullBlockSize;
            static_cast<fixed_stack_allocator*>(_pThis->m_pContainer)->m_ullCapacity = static_cast<fixed_stack_allocator*>(_stack.m_pContainer)->m_ullCapacity;
            static_cast<fixed_stack_allocator*>(_pThis->m_pContainer)->m_ullUsed = 0;

            for(uint32_t _i = 0; _i < _stack.GetSize(); ++_i)
            {
                Type* _v = reinterpret_cast<Type*>(reinterpret_cast<uint8_t*>(_stack.m_pContainer) + sizeof(fixed_stack_allocator) + (_i * sizeof(Type)));

                fixed_stack_allocate<Type>(_pThis->m_pMemoryInterface, _pThis->m_pContainer, *_v);
            }

            _pThis->m_fnCopy = _stack.m_fnCopy;
            _pThis->m_fnDestruct = _stack.m_fnDestruct;
            _pThis->m_pAllocator = _stack.m_pAllocator;
        }

        static void free_list_destruct(allocator* _pAllocator, SContainer* _pThis)
        {
            while(!fixed_stack_empty(_pThis->m_pMemoryInterface, _pThis->m_pContainer))
            {
                fixed_stack_pop<Type>(_pThis->m_pMemoryInterface, _pThis->m_pContainer);
            }

            free_list_free(_pThis->m_pMemoryInterface, static_cast<free_list_allocator*>(_pAllocator), _pThis->m_pContainer);
        }

        static void linear_destruct(allocator* _pAllocator, SContainer* _pThis)
        {
            // Can not be destructed in this way, we could only clear whole linear allocator
        }

    public:
        Stack()
        {

        }

        Stack(ftable* _pMemoryInterface, linear_allocator* _pAllocator, std::size_t _ullCount) :
            SContainer(_pMemoryInterface, _pAllocator)
        {
            this->m_pMemoryInterface = _pMemoryInterface;
            this->m_pAllocator = _pAllocator;
            this->m_fnDestruct = &linear_destruct;
            this->m_pContainer = this->m_pMemoryInterface->m_fnLinearAllocateFixedStackAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Stack(ftable* _pMemoryInterface, free_list_allocator* _pAllocator, std::size_t _ullCount) :
            SContainer(_pMemoryInterface, _pAllocator)
        {
            this->m_fnCopy = &free_list_copy;
            this->m_fnDestruct = &free_list_destruct;
            this->m_pContainer = _pMemoryInterface->m_fnFreeListAllocateFixedStackAllocator(_pMemoryInterface, _pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        }

        Stack(const Stack& _stack) :
            SContainer(_stack.m_pMemoryInterface, _stack.m_pAllocator)
        {
            _stack.m_fnCopy(_stack, this);
        }

        Stack(Stack&& _stack) noexcept :
            SContainer(
                std::move(_stack.m_pMemoryInterface),
                std::move(_stack.m_pAllocator),
                std::move(_stack.m_pContainer),
                std::move(_stack.m_fnCopy),
                std::move(_stack.m_fnDestruct)
            )
        {
            _stack.m_pMemoryInterface = nullptr;
            _stack.m_pContainer = nullptr;
            _stack.m_pAllocator = nullptr;
            _stack.m_fnCopy = nullptr;
            _stack.m_fnDestruct = nullptr;
        }

        ~Stack()
        {
            if(this->m_pAllocator == nullptr || this->m_pContainer == nullptr)
            {
                return;
            }

            this->m_fnDestruct(this->m_pAllocator, this);

            this->m_pAllocator = nullptr;
            this->m_pContainer = nullptr;
        }

        Stack& operator=(Stack&& _stack)
        {
            if(&_stack == this)
            {
                return *this;
            }

            this->m_pContainer = std::move(_stack.m_pContainer);
            this->m_pMemoryInterface = std::move(_stack.m_pMemoryInterface);
            this->m_fnCopy = std::move(_stack.m_fnCopy);
            this->m_fnDestruct = std::move(_stack.m_fnDestruct);
            this->m_pAllocator = std::move(_stack.m_pAllocator);

            _stack.m_pContainer = nullptr;
            _stack.m_pMemoryInterface = nullptr;
            _stack.m_fnCopy = nullptr;
            _stack.m_fnDestruct = nullptr;
            _stack.m_pAllocator = nullptr;

            return *this;
        }

        Stack& operator=(const Stack& _stack)
        {
            this->m_pMemoryInterface = _stack.m_pMemoryInterface;

            _stack.m_fnCopy(_stack, this);

            return *this;
        }

        // Stack()
        // {
        //     m_pContainer = nullptr;
        //     m_pAllocator = nullptr;
        //     m_pMemoryInterface = nullptr;
        // }

        // Stack(ftable* _pMemoryInterface, linear_allocator* _pAllocator, std::size_t _ullCount) :
        //     m_pMemoryInterface(_pMemoryInterface)
        // {
        //     m_pContainer = m_pMemoryInterface->m_fnLinearAllocateFixedStackAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        // }

        // Stack(ftable* _pMemoryInterface, free_list_allocator* _pAllocator, std::size_t _ullCount) :
        //     m_pMemoryInterface(_pMemoryInterface)
        // {
        //     m_pContainer = m_pMemoryInterface->m_fnFreeListAllocateFixedStackAllocator(_pAllocator, _ullCount * sizeof(Type), sizeof(Type));
        // }

        // Stack(ftable* _pMemoryInterface, fixed_stack_allocator* _pAllocator) :
        //     m_pMemoryInterface(_pMemoryInterface),
        //     m_pContainer(_pAllocator)
        // {

        // }

        // ~Stack()
        // {

        // }

        inline Type* Allocate(const Type& _data)
        {
            return fixed_stack_allocate(this->m_pMemoryInterface, this->m_pContainer, _data);
        }

        inline Type* Allocate(Type&& _data)
        {
            return fixed_stack_allocate(this->m_pMemoryInterface, this->m_pContainer, std::forward<Type>(_data));
        }

        inline const Type& Top() const
        {
            return *(Type*)fixed_stack_top(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline void Pop()
        {
            fixed_stack_pop(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline bool Empty() const
        {
            return fixed_stack_empty(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline bool Full() const
        {
            return fixed_stack_full(this->m_pMemoryInterface, this->m_pContainer);
        }

        inline std::size_t GetSize() const
        {
            return fixed_stack_size(this->m_pMemoryInterface, this->m_pContainer) / sizeof(Type);
        }

        inline std::size_t GetCapacity() const
        {
            return fixed_stack_capacity(this->m_pMemoryInterface, this->m_pContainer) / sizeof(Type);
        }
    };

}}