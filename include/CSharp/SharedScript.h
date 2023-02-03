#pragma once

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"

#include "ScriptEngine/Language.h"

#include "RuntimeReflection/Markers.h"

#include "CSharp/ReflectionFlags.h"

#include "RuntimeReflection/Resources.h"

#include "CSharp/SharedScript.generated.h"

#ifndef DUCKVIL_AST
#define DUCKVIL_CS_FUNCTION(rType, name, args) \
    struct duckvil_cs_ ## name args; \
    rType name(duckvil_cs_ ## name _args) { generated_csharp_ ## name(_args); }
#else
#define DUCKVIL_STRING(s) #s
#define DUCKVIL_CS_FUNCTION(rType, name, _args) class [[using duckvil: meta(rType,name,DUCKVIL_STRING(_args))]] __duckvil_csharp_function__ ## name;
#endif

namespace Duckvil { namespace CSharp {

    // Use _an_ prefix with CSharp::ReflectionFlags::EXPOSE_TO_CSHARP attribute to synchronize variable
    // To set or get value of the variable use methods without _an_ prefix (int _an_v -> v()/v(1))
    struct SharedScript
    {
        RuntimeReflection::__duckvil_resource_type_t m_typeHandle;

        // I guess it should be protected in order to be called as needed
        // Accessable by ILanguage/Language
        virtual void InternalInit() { }
        virtual bool Init() { throw std::exception(""); }
        virtual void Update()
        {

        }

        virtual void SetCSharpClass(void* _pClass)
        {

        }

        virtual void SetCSharpObject(void* _pObject)
        {

        }
    };

}}