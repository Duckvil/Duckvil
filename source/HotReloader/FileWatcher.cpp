#include "HotReloader/FileWatcher.h"

namespace Duckvil { namespace HotReloader {

    FileWatcher::FileWatcher(ActionCallback _action, void* _pUserData) :
        m_fnAction(_action),
        m_pUserData(_pUserData)
    {

    }

    FileWatcher::~FileWatcher()
    {

    }

    void FileWatcher::Watch(const std::filesystem::path& _path)
    {
        m_aPaths.push_back(_path);

        for(auto& _file : std::filesystem::recursive_directory_iterator(_path))
        {
            m_aPathsToWatch[_file.path().string()] = std::filesystem::last_write_time(_file);
        }
    }

    void FileWatcher::Update()
    {
        auto _it = m_aPathsToWatch.begin();

        while(_it != m_aPathsToWatch.end())
        {
            if(!std::filesystem::exists(_it->first))
            {
                m_fnAction(_it->first, FileStatus::FileStatus_Removed, m_pUserData);

                _it = m_aPathsToWatch.erase(_it);
            }
            else
            {
                _it++;
            }
        }

        for(auto& _path : m_aPaths)
        {
            for(auto& _file : std::filesystem::recursive_directory_iterator(_path))
            {
                const auto& _lastWriteTime = std::filesystem::last_write_time(_file);
                const std::string& _fileString = _file.path().string();

                if(!m_aPathsToWatch.contains(_fileString))
                {
                    m_aPathsToWatch[_fileString] = _lastWriteTime;

                    m_fnAction(_file, FileStatus::FileStatus_Created, m_pUserData);
                }
                else
                {
                    auto& _current = m_aPathsToWatch[_fileString];

                    if(_current != _lastWriteTime)
                    {
                        _current = _lastWriteTime;

                        m_fnAction(_file, FileStatus::FileStatus_Modified, m_pUserData);
                    }
                }
            }
        }
    }

}}