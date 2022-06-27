#include "Project.h"

namespace {$projectName} {

	Project::Project(const Duckvil::Memory::FreeList& _heap, const Duckvil::Memory::ThreadsafeVector<duckvil_recorderd_types>* _pSystems, const Duckvil::PlugNPlay::__module_information& _module) :
		m_heap(_heap),
		m_pSystems(_pSystems),
		m_module(_module)
	{
		// m_heap.Allocate(m_aSystems, 1);

		// for(const duckvil_recorderd_types& _system : _aSystems)
        // {
        //     if(m_aSystems.Full())
        //     {
        //         m_aSystems.Resize(m_aSystems.Size() * 2);
        //     }

		//     m_aSystems.Allocate(_system);
		// }

		_heap.Allocate(m_pScripts, 1);
	}

	Project::~Project()
	{

	}

	bool Project::Init(Duckvil::Event::Pool<Duckvil::Event::mode::immediate>* _pPMEventPool)
	{
		_pPMEventPool->AddA<Duckvil::ProjectManager::AddScriptEvent>([&](const Duckvil::ProjectManager::AddScriptEvent& _e)
		{
			if(m_pScripts.Full())
			{
				m_pScripts.Resize(m_pScripts.Size() * 2);
			}

			m_pScripts.Allocate(_e.m_pTrackKeepedScript);
		});

		for(const duckvil_recorderd_types& _type : *m_pSystems)
		{
			for(uint32_t _i = 0; _i < _type.m_ullCount; ++_i)
			{
				if(Duckvil::RuntimeReflection::inherits<Duckvil::Project::Script>(_type.m_aTypes[_i]))
				{
					void* _script = Duckvil::RuntimeReflection::create(m_heap, _type.m_aTypes[_i], true);

					if(_script != nullptr && static_cast<Duckvil::Project::Script*>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_script))->Init())
					{
						if(m_pScripts.Full())
						{
							m_pScripts.Resize(m_pScripts.Size() * 2);
						}

						m_pScripts.Allocate(static_cast<Duckvil::HotReloader::ITrackKeeper*>(_script));
					}
				}
			}
		}

		printf("Hello from {$projectName}!");

		// for(const duckvil_recorderd_types& _system : m_aSystems)
		// {
		// 	for(uint32_t i = 0; i < _system.m_ullCount; ++i)
		// 	{
		// 		const Duckvil::RuntimeReflection::__duckvil_resource_type_t& _type = _system.m_aTypes[i];
		// 		const Duckvil::RuntimeReflection::__variant& _variant = Duckvil::RuntimeReflection::get_meta(_type, Duckvil::ProjectManager::project_type::standalone);

		// 		if(_variant.m_ullTypeID == -1)
		// 		{

		// 		}
		// 	}
		// }

		return true;
	}

	void Project::Update()
	{
		for(const auto& _script : m_pScripts)
		{
			static_cast<Duckvil::Project::Script*>(DUCKVIL_TRACK_KEEPER_GET_OBJECT(_script))->Update();
		}
	}

}