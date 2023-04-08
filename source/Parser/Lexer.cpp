#include "Parser/Lexer.h"

#include <fstream>

#include "Parser/config.h"

namespace Duckvil { namespace Parser {

    void load_file(__lexer_data* _pData, const char _sFilename[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH])
    {
        _pData->m_uiCurrentLine = 0;
        _pData->m_uiCurrentCharacterIndex = 0;
        _pData->m_bSpace = false;
        _pData->m_bWasSpace = false;
        _pData->m_bNewLine = false;
        _pData->m_bEnd = false;

        std::ifstream _hFile(_sFilename);
        std::string _buf;

        while(std::getline(_hFile, _buf))
        {
            _pData->m_aLines.push_back(_buf);
        }

        _hFile.close();

        if(_pData->m_aLines.empty())
        {
            throw blank_file();
        }

        _pData->m_sCurrentLine = _pData->m_aLines[_pData->m_uiCurrentLine];
    }

    bool next_token(__lexer_data* _pData, std::string* _pToken)
    {
        if(_pData->m_bEnd)
        {
            return false;
        }

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

        while(1)
        {
            if(_pData->m_uiCurrentCharacterIndex >= _pData->m_sCurrentLine.size())
            {
                if(_pData->m_uiCurrentLine >= _pData->m_aLines.size() - 1)
                {
                    _pData->m_bEnd = true;

                    break;
                }

                _pData->m_sCurrentLine = _pData->m_aLines[++_pData->m_uiCurrentLine];
                _pData->m_uiCurrentCharacterIndex = 0;
                _pData->m_bNewLine = true;

                break;
            }

            _pData->m_cCurrentCharacter = _pData->m_sCurrentLine[_pData->m_uiCurrentCharacterIndex++];

            if(_pData->m_cCurrentCharacter == '\'' ||
                _pData->m_cCurrentCharacter == '\\' ||
                _pData->m_cCurrentCharacter == '{' ||
                _pData->m_cCurrentCharacter == '}' ||
                _pData->m_cCurrentCharacter == '[' ||
                _pData->m_cCurrentCharacter == ']' ||
                _pData->m_cCurrentCharacter == '<' ||
                _pData->m_cCurrentCharacter == '>' ||
                _pData->m_cCurrentCharacter == ')' ||
                _pData->m_cCurrentCharacter == '(' ||
                _pData->m_cCurrentCharacter == ':' ||
                _pData->m_cCurrentCharacter == ';' ||
                _pData->m_cCurrentCharacter == ',' ||
                _pData->m_cCurrentCharacter == '.' ||
                _pData->m_cCurrentCharacter == '&' ||
                _pData->m_cCurrentCharacter == '*' ||
                _pData->m_cCurrentCharacter == '/' ||
                _pData->m_cCurrentCharacter == '"' ||
                _pData->m_cCurrentCharacter == '#' ||
                _pData->m_cCurrentCharacter == '!' ||
                _pData->m_cCurrentCharacter == '-' ||
                _pData->m_cCurrentCharacter == '~')
            {
                if(_pData->m_sToken.size() > 0)
                {
                    _pData->m_uiCurrentCharacterIndex--;
                }
                else
                {
                    _pData->m_sToken += _pData->m_cCurrentCharacter;
                }

                break;
            }
            else if(_pData->m_cCurrentCharacter == ' ')
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

    IConfig* init(__lexer_data* _pData)
    {
        auto _config = new compile_config();

        _pData->m_pConfig = _config;

        _config->fast_preprocessing(true);
        _config->set_flags(cppast::cpp_standard::cpp_2a);

        return _config;
    }

}}

Duckvil::Parser::__lexer_ftable* duckvil_lexer_init()
{
    static Duckvil::Parser::__lexer_ftable _ftable = { 0 };

    _ftable.load_file = &Duckvil::Parser::load_file;
    _ftable.next_token = &Duckvil::Parser::next_token;
    _ftable.space = &Duckvil::Parser::space;
    _ftable.init = &Duckvil::Parser::init;

    return &_ftable;
}