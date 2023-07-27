#include "Memory/Memory.h"

namespace Duckvil { namespace Memory {

    bool impl_byte_buffer_will_fit(byte_buffer_allocator* _pAllocator, std::size_t _ullSize);
    void impl_byte_buffer_clear(byte_buffer_allocator* _pAllocator);
    void impl_byte_buffer_resize(const void* _pInterface, free_list_allocator* _pParentAllocator, byte_buffer_allocator** _pAllocator, std::size_t _ullNewSize);
    void impl_byte_buffer_seek_to_begin(byte_buffer_allocator* _pAllocator);
    void impl_byte_buffer_seek_to_end(byte_buffer_allocator* _pAllocator);
    void impl_byte_buffer_advance(byte_buffer_allocator* _pAllocator, std::size_t _ullSize);
    void impl_byte_buffer_write(byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize, const void* _pValue);
    void* impl_byte_buffer_read(byte_buffer_allocator* _pAllocator, std::size_t _ullTypeSize);

}}