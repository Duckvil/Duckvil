#include "Parser/Tokenizer.h"

#include <fstream>

namespace Duckvil { namespace Parser {

    void load_file(__loaded_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH])
    {
        _pData->m_uiCurrentLine = 0;
        _pData->m_uiCurrentCharacterIndex = 0;
        _pData->m_bEnd = false;

        std::ifstream _hFile(_sFilename);
        std::string _buf;

        while(std::getline(_hFile, _buf))
        {
            _pData->m_aLines.push_back(_buf);
        }

        _hFile.close();

        _pData->m_sCurrentLine = _pData->m_aLines[_pData->m_uiCurrentLine];
    }

    std::string get_token(__loaded_data* _pData)
    {
        _pData->m_sToken.clear();
        _pData->m_bWasSpace = false;
        _pData->m_cPendingSymbol = 0;

        while(1)
        {
            if(_pData->m_uiCurrentLine >= _pData->m_aLines.size() && _pData->m_uiCurrentCharacterIndex >= _pData->m_sCurrentLine.size())
            {
                _pData->m_bEnd = true;
            }

            _pData->m_cCurrentCharacter = _pData->m_sCurrentLine[_pData->m_uiCurrentCharacterIndex++];

            if(_pData->m_cCurrentCharacter == '&' ||
                _pData->m_cCurrentCharacter == '{' ||
                _pData->m_cCurrentCharacter == '}' ||
                _pData->m_cCurrentCharacter == '[' ||
                _pData->m_cCurrentCharacter == ']' ||
                _pData->m_cCurrentCharacter == ':' ||
                _pData->m_cCurrentCharacter == ';')
            {
                _pData->m_cPendingSymbol = _pData->m_cCurrentCharacter;

                break;
            }

            if(_pData->m_cCurrentCharacter == ' ')
            {
                _pData->m_bWasSpace = true;

                break;
            }

            _pData->m_sToken += _pData->m_cCurrentCharacter;

            if (_pData->m_uiCurrentCharacterIndex >= _pData->m_sCurrentLine.size())
            {
                _pData->m_sCurrentLine = _pData->m_aLines[++_pData->m_uiCurrentLine];
                _pData->m_uiCurrentCharacterIndex = 0;

                break;
            }
        }

        return _pData->m_sToken;
    }

    bool token_exists(const __loaded_data& _pData)
    {
        return _pData.m_bEnd;
    }

}}

Duckvil::Parser::__functions* duckvil_parser_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Parser::__functions* _funcs = (Duckvil::Parser::__functions*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Parser::__functions), alignof(Duckvil::Parser::__functions));

    _funcs->load_file = &Duckvil::Parser::load_file;
    _funcs->get_token = &Duckvil::Parser::get_token;
    _funcs->token_exists = &Duckvil::Parser::token_exists;

    return _funcs;
}