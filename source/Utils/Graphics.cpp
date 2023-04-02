#include "Utils/Graphics.h"

#include "glew/include/GL/glew.h"

namespace Duckvil { namespace Utils { namespace Graphics {

	void FrameBufferReader::Copy(const FrameBufferReader* _pFrom, FrameBufferReader* _pTo)
	{
        _pTo->m_u32Index = _pFrom->m_u32Index;
        _pTo->m_pFence = _pFrom->m_pFence;
        _pTo->m_pu32FrameBuffer = _pFrom->m_pu32FrameBuffer;
        _pTo->m_pu32Pbo = _pFrom->m_pu32Pbo;
        _pTo->m_pu32Texture = _pFrom->m_pu32Texture;
        _pTo->m_u8Channels = _pFrom->m_u8Channels;
        _pTo->m_u8Frames = _pFrom->m_u8Frames;
        _pTo->m_u32TargetWidth = _pFrom->m_u32TargetWidth;
        _pTo->m_u32TargetHeight = _pFrom->m_u32TargetHeight;
        _pTo->m_pvecQueue = _pFrom->m_pvecQueue;
        _pTo->m_pvecInUse = _pFrom->m_pvecInUse;
        _pTo->m_format = _pFrom->m_format;
        _pTo->m_type = _pFrom->m_type;
	}

	FrameBufferReader::FrameBufferReader()
	{
		
	}

	FrameBufferReader::FrameBufferReader(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, uint32_t _u32TargetWidth, uint32_t _u32TargetHeight, Format _format, Type _type) :
        m_u32TargetWidth(_u32TargetWidth),
        m_u32TargetHeight(_u32TargetHeight),
	    m_format(_format),
	    m_type(_type)
    {
        m_u8Channels = 4;
        m_u8Frames = 4;

        m_pu32Texture = new uint32_t[m_u8Frames];
        m_pu32FrameBuffer = new uint32_t[m_u8Frames];
        m_pu32Pbo = new uint32_t[m_u8Frames];
        m_pFence = new GLsync[m_u8Frames];

	    glGenTextures(m_u8Frames, m_pu32Texture);
        glGenFramebuffers(m_u8Frames, m_pu32FrameBuffer);
        glGenBuffers(m_u8Frames, m_pu32Pbo);

        GLenum _glFormat;
        GLenum _glType;

        switch(_format)
        {
        case RGBA:
            _glFormat = GL_RGBA;
            break;
        case RED_INTEGER:
            _glFormat = GL_RED_INTEGER;
            break;
        }

        switch(_type)
        {
        case UNSIGNED_BYTE:
            _glType = GL_UNSIGNED_BYTE;
            break;
        case UNSIGNED_INT:
            _glType = GL_UNSIGNED_INT;
            break;
        }

        for(int i = 0; i < m_u8Frames; i++)
        {
            glBindTexture(GL_TEXTURE_2D, m_pu32Texture[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _u32TargetWidth, _u32TargetHeight, 0, _glFormat, _glType, nullptr);
            glBindFramebuffer(GL_FRAMEBUFFER, m_pu32FrameBuffer[i]);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pu32Texture[i], 0);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pu32Pbo[i]);
            glBufferData(GL_PIXEL_PACK_BUFFER, _u32TargetWidth * _u32TargetHeight * m_u8Channels, nullptr, GL_STREAM_READ);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        m_pvecQueue = static_cast<Memory::Vector<uint32_t>*>(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Memory::Vector<uint32_t>), alignof(Memory::Vector<uint32_t>)));

        *m_pvecQueue = Memory::Vector<uint32_t>(_pMemoryInterface, _pAllocator, 4);

        m_pvecInUse = static_cast<Memory::Vector<uint32_t>*>(_pMemoryInterface->m_fnFreeListAllocate_(_pAllocator, sizeof(Memory::Vector<uint32_t>), alignof(Memory::Vector<uint32_t>)));

        *m_pvecInUse = Memory::Vector<uint32_t>(_pMemoryInterface, _pAllocator, 4);
    }

    FrameBufferReader::FrameBufferReader(const FrameBufferReader& _other)
	{
        Copy(&_other, this);
	}

    FrameBufferReader::FrameBufferReader(FrameBufferReader&& _other)
    {
        Copy(&_other, this);
    }

    FrameBufferReader::~FrameBufferReader()
    {
	    
    }

    FrameBufferReader& FrameBufferReader::operator=(const FrameBufferReader& _other)
	{
        if(this == &_other)
        {
            return *this;
        }

        Copy(&_other, this);
	}

    FrameBufferReader& FrameBufferReader::operator=(FrameBufferReader&& _other)
    {
        if(this == &_other)
        {
            return *this;
        }

        Copy(&_other, this);
    }

    void FrameBufferReader::Read(uint32_t _uiFBO, uint32_t _u32SourceWidth, uint32_t _u32SourceHeight, uint8_t _u8Attachment)
    {
        if(_u32SourceWidth < m_u32TargetWidth || _u32SourceHeight < m_u32TargetHeight)
        {
            return;
        }

        m_pvecInUse->Allocate(_uiFBO);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, _uiFBO);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + _u8Attachment);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pu32FrameBuffer[m_u32Index]);
        glBlitFramebuffer(0, 0, _u32SourceWidth, _u32SourceHeight, 0, 0, m_u32TargetWidth, m_u32TargetHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        GLenum _glFormat;
        GLenum _glType;

        switch(m_format)
        {
        case RGBA:
            _glFormat = GL_RGBA;
            break;
        case RED_INTEGER:
            _glFormat = GL_RED_INTEGER;
            break;
        }

        switch(m_type)
        {
        case UNSIGNED_BYTE:
            _glType = GL_UNSIGNED_BYTE;
            break;
        case UNSIGNED_INT:
            _glType = GL_UNSIGNED_INT;
            break;
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_pu32FrameBuffer[m_u32Index]);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pu32Pbo[m_u32Index]);
        glReadPixels(0, 0, m_u32TargetWidth, m_u32TargetHeight, _glFormat, _glType, nullptr);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

        m_pFence[m_u32Index] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        m_pvecQueue->Allocate(m_u32Index);
        m_u32Index = (m_u32Index + 1) % m_u8Frames;
    }

    void FrameBufferReader::Update(void _fnProcess(void* _pTextureData, uint32_t _uiWidth, uint32_t _uiHeight, uint32_t _uiOffset))
    {
        while(!m_pvecQueue->Empty())
        {
            const auto _index = m_pvecQueue->Begin();

            if(glClientWaitSync(m_pFence[_index], 0, 0) == GL_TIMEOUT_EXPIRED)
                break;

            glDeleteSync(m_pFence[_index]);
            glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pu32Pbo[_index]);

            auto ptr = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, m_u32TargetWidth * m_u32TargetHeight * m_u8Channels, GL_MAP_READ_BIT);
            
            _fnProcess(ptr, m_u32TargetWidth, m_u32TargetHeight, m_pvecQueue->Size());
            glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
            
            m_pvecQueue->Erase(0);
            m_pvecInUse->Erase(0);
        }
    }

}}}