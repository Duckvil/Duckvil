#include "UniTestFramework/UniTestFramework.h"

#include "test/TestMemory/Module.h"

#include "Memory/ByteBuffer.h"

#include <cstring>

DUCKVIL_TEST(ByteBufferAllocation)
{
    {
        Duckvil::Memory::linear_allocator* _linear = __duckvil_global::m_pMemoryInterface->m_fnLinearAllocateLinearAllocator((Duckvil::Memory::linear_allocator*)__duckvil_global::m_pMemoryChunk, 1024 * 1024 * 1024);

        DUCKVIL_TEST_IS_NULL(_linear, "Hmm, allocated too much memory");
    }

    {
        Duckvil::Memory::byte_buffer_allocator* _byteBuffer= __duckvil_global::m_pMemoryInterface->m_fnLinearAllocateByteBufferAllocator((Duckvil::Memory::linear_allocator*)__duckvil_global::m_pMemoryChunk, 16);

        {
            DUCKVIL_TEST_EXP(Duckvil::Memory::byte_buffer_will_fit<int>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should fit in");
            Duckvil::Memory::byte_buffer_write(__duckvil_global::m_pMemoryInterface, _byteBuffer, 10);

            DUCKVIL_TEST_EXP(Duckvil::Memory::byte_buffer_will_fit<int>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should fit in");
            Duckvil::Memory::byte_buffer_write(__duckvil_global::m_pMemoryInterface, _byteBuffer, 20);

            DUCKVIL_TEST_EXP(Duckvil::Memory::byte_buffer_will_fit<int>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should fit in");
            Duckvil::Memory::byte_buffer_write(__duckvil_global::m_pMemoryInterface, _byteBuffer, 30);

            DUCKVIL_TEST_EXP(Duckvil::Memory::byte_buffer_will_fit<int>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should fit in");
            Duckvil::Memory::byte_buffer_write(__duckvil_global::m_pMemoryInterface, _byteBuffer, 40);

            DUCKVIL_TEST_EXP(!Duckvil::Memory::byte_buffer_will_fit<char>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should not fit in");
        }

        Duckvil::Memory::byte_buffer_seek_to_begin(__duckvil_global::m_pMemoryInterface, _byteBuffer);

        {
            int* _res = Duckvil::Memory::byte_buffer_read<int>(__duckvil_global::m_pMemoryInterface, _byteBuffer);

            DUCKVIL_TEST_IS_NOT_NULL(_res, "Failed to retrieve value");
            DUCKVIL_TEST_EQUAL(*_res, 10, "Wrong value");

            _res = Duckvil::Memory::byte_buffer_read<int>(__duckvil_global::m_pMemoryInterface, _byteBuffer);

            DUCKVIL_TEST_IS_NOT_NULL(_res, "Failed to retrieve value");
            DUCKVIL_TEST_EQUAL(*_res, 20, "Wrong value");

            _res = Duckvil::Memory::byte_buffer_read<int>(__duckvil_global::m_pMemoryInterface, _byteBuffer);

            DUCKVIL_TEST_IS_NOT_NULL(_res, "Failed to retrieve value");
            DUCKVIL_TEST_EQUAL(*_res, 30, "Wrong value");

            _res = Duckvil::Memory::byte_buffer_read<int>(__duckvil_global::m_pMemoryInterface, _byteBuffer);

            DUCKVIL_TEST_IS_NOT_NULL(_res, "Failed to retrieve value");
            DUCKVIL_TEST_EQUAL(*_res, 40, "Wrong value");
        }

        Duckvil::Memory::byte_buffer_clear(__duckvil_global::m_pMemoryInterface, _byteBuffer);

        {
            DUCKVIL_TEST_EXP(Duckvil::Memory::byte_buffer_will_fit<char>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should fit in");
            Duckvil::Memory::byte_buffer_write(__duckvil_global::m_pMemoryInterface, _byteBuffer, 'a');

            DUCKVIL_TEST_EXP(Duckvil::Memory::byte_buffer_will_fit<char>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should fit in");
            Duckvil::Memory::byte_buffer_write(__duckvil_global::m_pMemoryInterface, _byteBuffer, 'b');

            DUCKVIL_TEST_EXP(Duckvil::Memory::byte_buffer_will_fit<float>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should fit in");
            Duckvil::Memory::byte_buffer_write(__duckvil_global::m_pMemoryInterface, _byteBuffer, 20.2f);

            DUCKVIL_TEST_EXP(Duckvil::Memory::byte_buffer_will_fit<int*>(__duckvil_global::m_pMemoryInterface, _byteBuffer), "Should fit in");
            Duckvil::Memory::byte_buffer_write(__duckvil_global::m_pMemoryInterface, _byteBuffer, new int(30));
        }

        Duckvil::Memory::byte_buffer_seek_to_begin(__duckvil_global::m_pMemoryInterface, _byteBuffer);

        {
            char* _res = Duckvil::Memory::byte_buffer_read<char>(__duckvil_global::m_pMemoryInterface, _byteBuffer);

            DUCKVIL_TEST_IS_NOT_NULL(_res, "Failed to retrieve value");
            DUCKVIL_TEST_EQUAL(*_res, 'a', "Wrong value");

            char* _res2 = Duckvil::Memory::byte_buffer_read<char>(__duckvil_global::m_pMemoryInterface, _byteBuffer);

            DUCKVIL_TEST_IS_NOT_NULL(_res2, "Failed to retrieve value");
            DUCKVIL_TEST_EQUAL(*_res2, 'b', "Wrong value");

            float* _res3 = Duckvil::Memory::byte_buffer_read<float>(__duckvil_global::m_pMemoryInterface, _byteBuffer);

            DUCKVIL_TEST_IS_NOT_NULL(_res3, "Failed to retrieve value");
            DUCKVIL_TEST_EQUAL(*_res3, 20.2f, "Wrong value");

            int** _res4 = Duckvil::Memory::byte_buffer_read<int*>(__duckvil_global::m_pMemoryInterface, _byteBuffer);

            DUCKVIL_TEST_IS_NOT_NULL(_res4, "Failed to retrieve value");
            DUCKVIL_TEST_EQUAL(**_res4, 30, "Wrong value");
        }
    }

    DUCKVIL_TEST_SUCCESS_PASS;
}