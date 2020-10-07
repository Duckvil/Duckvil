#pragma once

#include "Utils/Macro.h"

#include <vector>
#include <string>

#include "Memory/Memory.h"

#define DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH 256

namespace Duckvil { namespace Parser {

    struct __lexer_data
    {
        __lexer_data() :
            m_uiCurrentCharacterIndex(0)
        {
            
        }

        std::vector<std::string> m_aLines;
        std::string m_sCurrentLine;
        uint32_t m_uiCurrentLine;
        uint32_t m_uiCurrentCharacterIndex;
        std::string m_sToken;
        char m_cCurrentCharacter;
        bool m_bWasSpace;
        bool m_bSpace;
        bool m_bNewLine;
        bool m_bEnd;
    };

    struct __lexer_ftable
    {
        void (*load_file)(__lexer_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH]);
        bool (*next_token)(__lexer_data* _pData, std::string* _pToken);
        bool (*space)(const __lexer_data& _data);
    };

    void load_file(__lexer_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH]);
    bool next_token(__lexer_data* _pData, std::string* _pToken);
    bool space(const __lexer_data& _data);

}}

DUCKVIL_EXPORT Duckvil::Parser::__lexer_ftable* duckvil_lexer_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);