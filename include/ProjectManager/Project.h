#pragma once

#include "RuntimeReflection/Recorder.h"

#include "Memory/Vector.h"

#include "Utils/Utils.h"

namespace Duckvil { namespace ProjectManager {

    struct project
    {
        Memory::Vector<duckvil_recorderd_types> m_aTypes;
        void* m_pObject;
        Utils::string m_sPath;
        PlugNPlay::__module_information m_module;

        RuntimeReflection::__duckvil_resource_type_t m_uiTypeHandleID;
        RuntimeReflection::__duckvil_resource_function_t m_uiInitFunctionHandleID;
        RuntimeReflection::__duckvil_resource_function_t m_uiUpdateFunctionHandleID;
    };

}}