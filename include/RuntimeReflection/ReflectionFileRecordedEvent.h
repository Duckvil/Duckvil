#pragma once

#include "RuntimeReflection/Recorder.h"

namespace Duckvil { namespace RuntimeReflection {

    struct ReflectionFileRecordedEvent
    {
        const duckvil_recorderd_types& m_types;

        ReflectionFileRecordedEvent(const duckvil_recorderd_types& _types) :
			m_types(_types)
        {
	        
        }
    };

}}