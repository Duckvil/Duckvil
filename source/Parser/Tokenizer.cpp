#include "Parser/Tokenizer.h"

#include <fstream>

namespace Duckvil { namespace Parser {

    void load_file(Memory::IMemory* m_pMemory, Memory::__free_list_allocator* _pAllocator, __loaded_data* _data, const char _sHeader[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH], const char _sSource[DUCKVIL_TOKENIZER_MAX_FILENAME_LENGTH])
    {
        std::ifstream _hFile(_sHeader);
        std::string _buf;

        while(std::getline(_hFile, _buf))
        {
            Utils::string _line(_buf.c_str(), _buf.size());

            _data->m_aHeaderLines.Allocate(_line);
        }

        _hFile.close();
    }

}}

Duckvil::Parser::__functions* duckvil_parser_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator)
{
    Duckvil::Parser::__functions* _funcs = (Duckvil::Parser::__functions*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(Duckvil::Parser::__functions), alignof(Duckvil::Parser::__functions));

    _funcs->load_file = &Duckvil::Parser::load_file;

    return _funcs;
}