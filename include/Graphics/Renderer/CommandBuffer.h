#pragma once

#include "Memory/ByteBuffer.h"

namespace Duckvil { namespace Graphics { namespace Renderer {

    struct command_buffer
    {
        Memory::byte_buffer_allocator* m_pCommands;
        uint32_t m_uiCommandCount;
    };

    inline static command_buffer command_buffer_new(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator)
    {
        command_buffer _commandBuffer = { 0 };

        _commandBuffer.m_pCommands = _pMemoryInterface->m_fnFreeListAllocateByteBufferAllocator(_pMemoryInterface, _pAllocator, 0);

        return _commandBuffer;
    }

    template <typename Type>
    inline static void command_buffer_write(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, command_buffer* _pCommandBuffer, const Type& _value)
    {
        if(!Memory::byte_buffer_will_fit<Type>(&_pMemoryInterface->m_byteBufferContainer, _pCommandBuffer->m_pCommands))
        {
            Memory::byte_buffer_resize(_pMemoryInterface, _pAllocator, &_pCommandBuffer->m_pCommands, _pCommandBuffer->m_pCommands->m_ullCapacity + sizeof(Type));
        }

        Memory::byte_buffer_write(&_pMemoryInterface->m_byteBufferContainer, _pCommandBuffer->m_pCommands, _value);
    }

    inline static void command_buffer_clear(const Memory::ftable* _pMemoryInterface, command_buffer* _pCommandBuffer)
    {
        Memory::byte_buffer_clear(&_pMemoryInterface->m_byteBufferContainer, _pCommandBuffer->m_pCommands);
    }

}}}