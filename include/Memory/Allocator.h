#pragma once

#include "Memory/Memory.h"

#include <memory>

namespace Duckvil { namespace Memory {

    bool allocate(__memory* _pMemory, size_t _ullSize)
    {
        if(_pMemory->memory == NULL)
        {
            _pMemory->memory = (uint8_t*)malloc(_ullSize);
            _pMemory->capacity = _ullSize;
        }

        return true;
    }

    void* allocate_linear(__memory* _pMemory, const void* _pData, size_t _ullSize)
    {
        void* _memory = (void*)(_pMemory->memory + _pMemory->used);

        memcpy(_memory, _pData, _ullSize);

        _pMemory->used += _ullSize;

        return _memory;
    }

    template <typename Type>
    bool allocate_linear(__memory* _pMemory, const Type& _data)
    {
        Type* _memory = (Type*)(_pMemory->memory + _pMemory->used);

        *_memory = _data;

        _pMemory->used += sizeof(Type);

        return true;
    }

}}