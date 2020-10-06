#pragma once

#include "Utils/Macro.h"

#include <vector>
#include <string>

#include "Memory/Memory.h"

#define DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH 256

namespace Duckvil { namespace Parser {

    struct __loaded_data
    {
        std::vector<std::string> m_aLines;
        std::string m_sCurrentLine;
        uint32_t m_uiCurrentLine;
        uint32_t m_uiCurrentCharacterIndex;
        std::string m_sToken;
        char m_cCurrentCharacter;
        bool m_bWasSpace;
        char m_cPendingSymbol;
        bool m_bEnd;
    };

    struct __functions
    {
        void (*load_file)(__loaded_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH]);
        std::string (*get_token)(__loaded_data* _pData);
        bool (*token_exists)(const __loaded_data& _pData);
    };

    void load_file(__loaded_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH]);
    std::string get_token(__loaded_data* _pData);
    bool token_exists(const __loaded_data& _pData);

}}

DUCKVIL_EXPORT Duckvil::Parser::__functions* duckvil_parser_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator);