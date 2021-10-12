#include "Project.h"

namespace {$projectName} {

	Project::Project(const Duckvil::Memory::FreeList& _heap, const Duckvil::Memory::Vector<duckvil_recorderd_types>& _aSystems) :
		m_heap(_heap)
	{
		m_heap.Allocate(m_aSystems, 1);

		for(const duckvil_recorderd_types& _system : _aSystems)
        {
            if(m_aSystems.Full())
            {
                m_aSystems.Resize(m_aSystems.Size() * 2);
            }

		    m_aSystems.Allocate(_system);
		}
	}

	Project::~Project()
	{

	}

	bool Project::Init()
	{
		printf("Hello from plugin!");

		for(const duckvil_recorderd_types& _system : m_aSystems)
		{
			for(uint32_t i = 0; i < _system.m_ullCount; ++i)
			{
				const Duckvil::RuntimeReflection::__duckvil_resource_type_t& _type = _system.m_aTypes[i];
				const Duckvil::RuntimeReflection::__variant& _variant = Duckvil::RuntimeReflection::get_meta(_type, Duckvil::ProjectManager::project_type::standalone);

				if(_variant.m_ullTypeID == -1)
				{

				}
			}
		}

		return true;
	}

}