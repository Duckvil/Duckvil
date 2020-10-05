#pragma once

#include "Memory/Vector.h"

#include "Utils/Utils.h"
#include "Utils/Macro.h"

#define DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH 256

namespace Duckvil { namespace Parser {

    struct __loaded_data
    {
        __loaded_data()
        {
            
        }

        Memory::Vector<Utils::string> m_aHeaderLines;
        Memory::Vector<Utils::string> m_aSourceLines;
    };

    struct __functions
    {
        void (*load_file)(Memory::IMemory* m_pMemory, Memory::__free_list_allocator* _pAllocator, __loaded_data* _data, const char _sHeader[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH], const char _sSource[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH]);
    };

    void load_file(Memory::IMemory* m_pMemory, Memory::__free_list_allocator* _pAllocator, __loaded_data* _data, const char _sHeader[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH], const char _sSource[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH]);

}}

DUCKVIL_EXPORT Duckvil::Parser::__functions* duckvil_parser_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);