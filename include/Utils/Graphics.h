#pragma once

#include "Memory/Vector.h"
#include "Memory/FreeList.h"

typedef struct __GLsync* GLsync;

namespace Duckvil { namespace Utils { namespace Graphics {

    class FrameBufferReader
    {
    public:
        enum Format
        {
            RED_INTEGER,
            RGBA
        };

        enum Type
        {
            UNSIGNED_INT,
            UNSIGNED_BYTE
        };

    private:
        uint32_t* m_pu32Texture;
        uint32_t* m_pu32FrameBuffer;
        uint32_t* m_pu32Pbo;
        GLsync* m_pFence;
        uint32_t m_u32Index = 0;
        //std::vector<int>* m_pvecQueue;
        Memory::Vector<uint32_t>* m_pvecQueue;
        Memory::Vector<uint32_t>* m_pvecInUse;

        uint32_t m_u32TargetWidth;
        uint32_t m_u32TargetHeight;

        uint8_t m_u8Channels;
        uint8_t m_u8Frames;

        Format m_format;
        Type m_type;

        static void Copy(const FrameBufferReader* _pFrom, FrameBufferReader* _pTo);

    public:
        FrameBufferReader();
        FrameBufferReader(const Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, uint32_t _u32TargetWidth, uint32_t _u32TargetHeight, Format _format, Type _type);
        FrameBufferReader(const FrameBufferReader&);
        FrameBufferReader(FrameBufferReader&&);
        ~FrameBufferReader();

        FrameBufferReader& operator=(const FrameBufferReader&);
        FrameBufferReader& operator=(FrameBufferReader&&);

        void Update(void (_fnProcess)(void* _pTextureData, uint32_t _uiWidth, uint32_t _uiHeight, uint32_t _uiOffset));
        void Read(uint32_t _uiFBO, uint32_t _u32SourceWidth, uint32_t _u32SourceHeight, uint8_t _u8Attachment);

        void Dispose();
    };

}}}