#include "ProjectManager/ProjectManager.h"

#include "PlugNPlay/Module.h"

#include "RuntimeReflection/RuntimeReflection.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Meta.h"

namespace Duckvil { namespace ProjectManager {

    bool init_project_manager(data* _pData, const Memory::FreeList& _heap)
    {
        _pData->m_heap = _heap;

        return true;
    }

    project load_project(data* _pData, const Utils::string& _sFilename, const Utils::string& _sPath)
    {
        project _project = { };

        _pData->m_heap.Allocate(_project.m_aTypes, 1);

        PlugNPlay::__module _module;

        PlugNPlay::module_init(&_module);

        PlugNPlay::__module_information _projectManager(_sFilename, _sPath);

        _module.load(&_projectManager);

        uint32_t (*get_recorder_count)();

        _module.get(_projectManager, "duckvil_get_runtime_reflection_recorder_count", reinterpret_cast<void**>(&get_recorder_count));

        uint32_t _recordersCount = get_recorder_count();

        for(uint32_t j = 0; j < _recordersCount; ++j)
        {
            duckvil_recorderd_types (*record)(Memory::ftable* _pMemoryInterface, Memory::free_list_allocator* _pAllocator, RuntimeReflection::__recorder_ftable* _pRecorder, RuntimeReflection::__ftable* _pRuntimeReflection, RuntimeReflection::__data* _pData);

            _module.get(_projectManager, (std::string("duckvil_runtime_reflection_record_") + std::to_string(j)).c_str(), reinterpret_cast<void**>(&record));

            if(record == nullptr)
            {
                // TODO: Should return false?

                continue;
            }

            duckvil_recorderd_types _types = record(_pData->m_heap.GetMemoryInterface(), _pData->m_heap.GetAllocator(), RuntimeReflection::get_current().m_pRecorder, RuntimeReflection::get_current().m_pReflection, RuntimeReflection::get_current().m_pReflectionData);

            if(_project.m_aTypes.Full())
            {
                _project.m_aTypes.Resize(_project.m_aTypes.Size() * 2);
            }

            _project.m_aTypes.Allocate(_types);
        }

        for(const auto& _type : _project.m_aTypes)
        {
            for(uint32_t i = 0; i < _type.m_ullCount; ++i)
            {
                const RuntimeReflection::__duckvil_resource_type_t& _typeID = _type.m_aTypes[i];
                const auto& _variant = RuntimeReflection::get_meta(_typeID, ProjectManager::project_type::standalone);

                if(_variant.m_ullTypeID != -1)
                {
                    _project.m_pObject = RuntimeReflection::create(_pData->m_heap, _typeID, false);
                }
            }
        }

        return _project;
    }

}}

void duckvil_project_manager_init(Duckvil::ProjectManager::ftable* _pFTable)
{
    _pFTable->m_fnInitProjectManager = &Duckvil::ProjectManager::init_project_manager;
    _pFTable->m_fnLoadProject = &Duckvil::ProjectManager::load_project;
}