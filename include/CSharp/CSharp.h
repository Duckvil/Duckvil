#pragma once

#define DUCKVIL_RUNTIME_REFLECTION_PAUSE
#define DUCKVIL_RUNTIME_REFLECTION_RESUME

#include "ScriptEngine/Language.h"

#include "Memory/FreeList.h"
#include "Memory/Vector.h"

#include "CSharp/SharedScript.h"

#include "Engine/EntityFactory.h"

#include "Event/ImmediatePool.h"

DUCKVIL_RUNTIME_REFLECTION_PAUSE

//extern "C" {
//    typedef struct _MonoClass MonoClass;
//    typedef struct _MonoObject MonoObject;
//    typedef struct _MonoMethod MonoMethod;
//    typedef struct _MonoAssembly MonoAssembly;
//    typedef struct _MonoImage MonoImage;
//    typedef struct _MonoClassField MonoClassField;
//}

DUCKVIL_RUNTIME_REFLECTION_RESUME

#define DUCKVIL_CSHARP_VARIABLE_CHANGED(_class, type, name)\
    static void csharp_internal_VariableChanged_ ## name(size_t _ullSharedScriptID, MonoString* _sName, MonoObject* _pValue)\
	{\
	    auto _csEngine = Duckvil::CSharp::CSharp::GetScriptEngine();\
	    _class* _script = static_cast<_class*>(_csEngine->GetSharedScript(_ullSharedScriptID));\
\
	    const char* _nameCstr = mono_string_to_utf8(_sName);\
	    std::string _name = _nameCstr;\
\
	    _script->_an_ ## name = *(type*)mono_object_unbox(_pValue);\
\
	    printf("Variable changed %s!\n", _nameCstr);\
	}

namespace Duckvil { namespace CSharp {

    class CSharp : public ScriptEngine::Language
    {
    public:
        struct Function
        {
            typedef bool (*HasComponentCallback)(Entity);
            typedef bool (*AddComponentCallback)(Entity, void*);

            Duckvil::CSharp::CSharp::Function::HasComponentCallback m_fnHasComponent;
            Duckvil::CSharp::CSharp::Function::AddComponentCallback m_fnAddComponent;
        };

        static const char* ms_sRootDomainName;
        static const char* ms_sAppDomainName;

        static Duckvil::CSharp::CSharp* GetScriptEngine();

        enum class ScriptFieldType
        {
	        None,
            Int8,
            UInt8,
            Int16,
            UInt16,
            Int32,
            UInt32,
            Int64,
            UInt64,
            SIZE
        };

        static std::unordered_map<std::string, Duckvil::CSharp::CSharp::ScriptFieldType> ms_aScriptFieldTypes;

    private:
        MonoDomain* m_pRootDomain;
        MonoDomain* m_pAppDomain;

        Memory::FreeList m_heap;

        // Memory::Vector<SharedScript*> m_aSharedScripts;
        std::unordered_map<size_t, SharedScript*> m_aSharedScripts;
        std::vector<size_t> m_aSharedScriptsFreeIDs;

        EntityFactory* m_pEntityFactory;

        Duckvil::Event::Pool<Duckvil::Event::mode::immediate>* m_pEngineEventPool;

        Duckvil::Event::Pool<Duckvil::Event::mode::immediate> m_csharpEventPool;

        char* ReadBytes(const char* _sFilePath, uint32_t* _uiOutSize);
        MonoAssembly* LoadAssembly(const char* _sFilePath);

    public:
        CSharp(const Memory::FreeList& _heap, EntityFactory* _pEntityFactory, DUCKVIL_ARGUMENT("Engine") Duckvil::Event::Pool<Duckvil::Event::mode::immediate>* _pEngineEventPool);
        ~CSharp();

        std::unordered_map<MonoType*, Duckvil::CSharp::CSharp::Function> m_aComponentFuncs;

        void* m_pCoreAssembly;
        void* m_pAppAssembly;

        bool Setup() override;
        bool Init() override;
        void Update() override;
        bool Shutdown() override;

        MonoObject* Create(void* _pClass);
        SharedScript* Create(const RuntimeReflection::__duckvil_resource_type_t& _type);

        void Register(size_t* _pullID, SharedScript* _pNativeClass);

        static Duckvil::CSharp::CSharp::ScriptFieldType MonoTypeToScriptFieldType(void* _pType);

        EntityFactory* GetECS() const
        {
            return m_pEntityFactory;
        }

        MonoDomain* GetAppDomain() const
        {
            return m_pAppDomain;
        }

        MonoDomain* GetRootDomain() const
        {
            return m_pRootDomain;
        }

        inline Duckvil::CSharp::SharedScript* GetSharedScript(size_t _ullID)
        {
            return m_aSharedScripts[_ullID];
        }

        void* GetType(const char* _sTypeName, const char** _saNamepsace, uint32_t _uiCount) override;
        void* CreateObject(void* _pType) override;
    };

}}