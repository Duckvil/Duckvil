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

    void AutoLoader::LoadAll()
    {
        for(auto& p : std::filesystem::directory_iterator(m_sLoadPath))
        {
            if(p.path().filename().extension() != ".dll")
            {
                continue;
            }

            // char _filename[64];
            // char _path[256];

            // memcpy(_filename, p.path().filename().string().c_str(), 64);
            // memcpy(_path, p.path().parent_path().string().c_str(), 256);

            std::string& _filename = p.path().filename().string();
            std::string& _path = p.path().parent_path().string();

            __module_information _module(Utils::string(_filename.c_str(), _filename.size() + 1));

            _module.m_sPath = Utils::string(_path.c_str(), _path.size() + 1);

            m_module.load(&_module);
        }
    }

}}