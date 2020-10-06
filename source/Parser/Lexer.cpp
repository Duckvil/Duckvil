#include "Parser/Lexer.h"

#include <fstream>

namespace Duckvil { namespace Parser {

    void load_file(__lexer_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH])
    {
        _pData->m_uiCurrentLine = 0;
        _pData->m_uiCurrentCharacterIndex = 0;
        _pData->m_bSpace = false;
        _pData->m_bWasSpace = false;
        _pData->m_bNewLine = false;

        std::ifstream _hFile(_sFilename);
        std::string _buf;

        while(std::getline(_hFile, _buf))
        {
            _pData->m_aLines.push_back(_buf);
        }

        _hFile.close();

        _pData->m_sCurrentLine = _pData->m_aLines[_pData->m_uiCurrentLine];
    }

    bool next_token(__lexer_data* _pData, std::string* _pToken)
    {
        _pData->m_sToken.clear();
        _pData->m_bSpace = false;
        _pData->m_bNewLine = false;

        if(_pData->m_bWasSpace)
        {
            _pData->m_bWasSpace = false;
            _pData->m_bSpace = true;

            *_pToken = _pData->m_sToken;

            return true;
        }

        // _pData->m_cPendingSymbol = 0;

        while(1)
        {
            if(_pData->m_uiCurrentLine >= _pData->m_aLines.size() - 1 && _pData->m_uiCurrentCharacterIndex >= _pData->m_sCurrentLine.size())
            {
                return false;
            }

            if(_pData->m_uiCurrentCharacterIndex >= _pData->m_sCurrentLine.size())
            {
                _pData->m_sCurrentLine = _pData->m_aLines[++_pData->m_uiCurrentLine];
                _pData->m_uiCurrentCharacterIndex = 0;
                // _pData->m_cPendingSymbol = '\n';
                // _pData->m_sToken = "\n";
                _pData->m_bNewLine = true;

                break;
            }

            _pData->m_cCurrentCharacter = _pData->m_sCurrentLine[_pData->m_uiCurrentCharacterIndex++];

            if(_pData->m_cCurrentCharacter == '\'' ||
                _pData->m_cCurrentCharacter == '}' ||
                _pData->m_cCurrentCharacter == '[' ||
                _pData->m_cCurrentCharacter == ']' ||
                _pData->m_cCurrentCharacter == '<' ||
                _pData->m_cCurrentCharacter == '>' ||
                _pData->m_cCurrentCharacter == ':' ||
                _pData->m_cCurrentCharacter == ';' ||
                _pData->m_cCurrentCharacter == ',' ||
                _pData->m_cCurrentCharacter == '.' ||
                _pData->m_cCurrentCharacter == '&' ||
                _pData->m_cCurrentCharacter == '*' ||
                _pData->m_cCurrentCharacter == '/' ||
                _pData->m_cCurrentCharacter == '"' ||
                _pData->m_cCurrentCharacter == '{' ||
                _pData->m_cCurrentCharacter == '#')
            {
                // _pData->m_cPendingSymbol = _pData->m_cCurrentCharacter;
                _pData->m_sToken += _pData->m_cCurrentCharacter;

                break;
            }

            if(_pData->m_cCurrentCharacter == ' ')
            {
                _pData->m_bWasSpace = true;

                break;
            }

            _pData->m_sToken += _pData->m_cCurrentCharacter;
        }

        *_pToken = _pData->m_sToken;

        return true;
    }

    bool space(const __lexer_data& _data)
    {
        return _data.m_bSpace;
    }

}}

Duckvil::Parser::__lexer_ftable* duckvil_lexer_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Parser::__lexer_ftable* _table = (Duckvil::Parser::__lexer_ftable*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Parser::__lexer_ftable), alignof(Duckvil::Parser::__lexer_ftable));

    _table->load_file = &Duckvil::Parser::load_file;
    _table->next_token = &Duckvil::Parser::next_token;
    _table->space = &Duckvil::Parser::space;

    return _table;
}