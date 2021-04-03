#include "PlugNPlay/AutoLoader.h"

namespace Duckvil { namespace PlugNPlay {

    AutoLoader::AutoLoader(const std::filesystem::path& _sLoadPath) :
        m_sLoadPath(_sLoadPath)
    {
        module_init(&m_module);
    }

    AutoLoader::~AutoLoader()
    {

    }

    void AutoLoader::LoadAll(Memory::ftable* _pMemory, Memory::free_list_allocator* _pAllocator, __module_information** _pModules, uint32_t* _pLoaded)
    {
        std::vector<__module_information> _toLoad;

        for(auto& p : std::filesystem::directory_iterator(m_sLoadPath))
        {
#ifdef DUCKVIL_PLATFORM_WINDOWS
            if(p.path().filename().extension() != ".dll")
            {
                continue;
            }
#else
#ifdef DUCKVIL_PLATFORM_LINUX
            if(p.path().filename().extension() != ".so")
            {
                continue;
            }
#endif
#endif

            std::string _filename = p.path().filename().string();
            const std::string& _path = p.path().parent_path().string();

            size_t _lastindex = _filename.find_last_of(".");
            _filename = _filename.substr(0, _lastindex);

            __module_information _module(Utils::string(_filename.c_str(), _filename.size() + 1));

            _module.m_sPath = Utils::string(_path.c_str(), _path.size() + 1);

            _toLoad.push_back(_module);
        }

        *_pModules = (__module_information*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(__module_information) * _toLoad.size(), alignof(__module_information));
        *_pLoaded = _toLoad.size();

        uint32_t _index = 0;

        for(__module_information _module : _toLoad)
        {
            m_module.load(&_module);

            (*_pModules)[_index] = _module;

            _index++;
        }
    }

}}