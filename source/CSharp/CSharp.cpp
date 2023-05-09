#include "CSharp/CSharp.h"

#include <fstream>

#include "CSharp/SharedScript.h"

#include "RuntimeReflection/RuntimeReflection.h"

#include "glm/glm.hpp"

#include "Graphics/TransformComponent.h"

#include "Engine/Events/RequestSystemEvent.h"

#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"

#include "CSharp/Events/MonoObjectCreatedEvent.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/reflection.h"

#define DUCKVIL_CS_ADD_INTERNAL_CALL(name) mono_add_internal_call("Duckvil.InternalCalls::"#name, name)

namespace Duckvil { namespace CSharp {

    const char* CSharp::ms_sRootDomainName = "DuckvilJIT_Runtime";
    const char* CSharp::ms_sAppDomainName = "DuckvilScriptRuntime";

    static CSharp* gs_pScriptEngine;

    std::unordered_map<std::string, CSharp::ScriptFieldType> CSharp::ms_aScriptFieldTypes =
    {
        { "System.Int8", CSharp::ScriptFieldType::Int8 },
        { "System.UInt8", CSharp::ScriptFieldType::UInt8 },
        { "System.Int16", CSharp::ScriptFieldType::Int16 },
        { "System.UInt16", CSharp::ScriptFieldType::UInt16 },
        { "System.Int32", CSharp::ScriptFieldType::Int32 },
        { "System.UInt32", CSharp::ScriptFieldType::UInt32 },
        { "System.Int64", CSharp::ScriptFieldType::Int64 },
        { "System.UInt64", CSharp::ScriptFieldType::UInt64 }
    };

    inline std::vector<std::string> cpp_combine(MonoClass* _pClass)
    {
        const char* _namespace = mono_class_get_namespace(_pClass);

        return Utils::split(_namespace, '.');
    }

    static bool entity_has_component(size_t _ullID, MonoReflectionType* _pComponentType)
    {
        MonoType* _type = mono_reflection_type_get_type(_pComponentType);
        ECS::Entity _entity = CSharp::GetScriptEngine()->GetECS()->FromID(_ullID);

        return CSharp::GetScriptEngine()->m_aComponentFuncs[_type].m_fnHasComponent(_entity);
    }

    static bool entity_add_component(size_t _ullID, MonoReflectionType* _pComponentType, void* _pData)
    {
        MonoType* _type = mono_reflection_type_get_type(_pComponentType);
        ECS::Entity _entity = CSharp::GetScriptEngine()->GetECS()->FromID(_ullID);

        return CSharp::GetScriptEngine()->m_aComponentFuncs[_type].m_fnAddComponent(_entity, _pData);
    }

    static void transform_component_set_translation(size_t _ullID, const glm::vec3* _pInTranslation)
    {
        ECS::Entity _entity = CSharp::GetScriptEngine()->GetECS()->FromID(_ullID);

        _entity.GetMut<Graphics::TransformComponent>()->m_position = *_pInTranslation;
    }

    static void transform_component_get_translation(size_t _ullID, glm::vec3* _pOutTranslation)
    {
        ECS::Entity _entity = CSharp::GetScriptEngine()->GetECS()->FromID(_ullID);

        *_pOutTranslation = _entity.GetMut<Graphics::TransformComponent>()->m_position;
    }

    static void transform_component_set_rotation(size_t _ullID, const glm::quat* _pInRotation)
    {
        ECS::Entity _entity = CSharp::GetScriptEngine()->GetECS()->FromID(_ullID);

        _entity.GetMut<Graphics::TransformComponent>()->m_rotation = *_pInRotation;
    }

    static void transform_component_get_rotation(size_t _ullID, glm::quat* _pOutRotation)
    {
        ECS::Entity _entity = CSharp::GetScriptEngine()->GetECS()->FromID(_ullID);

        *_pOutRotation = _entity.GetMut<Graphics::TransformComponent>()->m_rotation;
    }

    static void transform_component_set_scale(size_t _ullID, const glm::vec3* _pInScale)
    {
        ECS::Entity _entity = CSharp::GetScriptEngine()->GetECS()->FromID(_ullID);

        _entity.GetMut<Graphics::TransformComponent>()->m_scale = *_pInScale;
    }

    static void transform_component_get_scale(size_t _ullID, glm::vec3* _pOutScale)
    {
        ECS::Entity _entity = CSharp::GetScriptEngine()->GetECS()->FromID(_ullID);

        *_pOutScale = _entity.GetMut<Graphics::TransformComponent>()->m_scale;
    }

    static void shared_script_register(size_t* _ullID, MonoObject* _pObject)
    {
        CSharp* _engine = CSharp::GetScriptEngine();

        MonoClass* _class = mono_object_get_class(_pObject);
        MonoType* _type = mono_class_get_type(_class);

        MonoClass* _sharedScriptClass = mono_class_from_name(mono_assembly_get_image(static_cast<MonoAssembly*>(_engine->m_pCoreAssembly)), "Duckvil", "SharedScript");

        if(!mono_class_implements_interface(_class, _sharedScriptClass))
        {
            return;
        }

        const auto& _namespace = cpp_combine(_class);
        std::vector<const char*> _namespaceCstr;

        for(const auto& _n : _namespace)
        {
            _namespaceCstr.push_back(_n.c_str());
        }

        auto _typeHandle = RuntimeReflection::get_type(mono_class_get_name(_class), _namespaceCstr);

        SharedScript* _nativeScript = _engine->Create(_typeHandle);

        _engine->Register(_ullID, _nativeScript);

        _nativeScript->SetCSharpClass(_class);
        _nativeScript->SetCSharpObject(_pObject);
        _nativeScript->InternalInit();
    }

    CSharp* CSharp::GetScriptEngine()
    {
        return gs_pScriptEngine;
    }

    SharedScript* CSharp::Create(const RuntimeReflection::__duckvil_resource_type_t& _type)
    {
        auto _sharedScript = static_cast<SharedScript*>(RuntimeReflection::create(m_heap, _type, false));

        _sharedScript->m_typeHandle = _type;

        return _sharedScript;
    }

    void CSharp::Register(size_t* _pullID, SharedScript* _pNativeClass)
    {
        if(m_aSharedScriptsFreeIDs.empty())
        {
            *_pullID = m_aSharedScripts.size();

            m_aSharedScripts[*_pullID] = _pNativeClass;
        }
        else
        {
            *_pullID = m_aSharedScriptsFreeIDs.back();

            m_aSharedScripts[*_pullID] = _pNativeClass;

            m_aSharedScriptsFreeIDs.pop_back();
        }
    }

    CSharp::ScriptFieldType CSharp::MonoTypeToScriptFieldType(void* _pType)
    {
        std::string _typeName = mono_type_get_name(static_cast<MonoType*>(_pType));

        return ms_aScriptFieldTypes[_typeName];
    }

    char* CSharp::ReadBytes(const char* _sFilePath, uint32_t* _uiOutSize)
    {
        std::ifstream _iStream(_sFilePath, std::ios::binary | std::ios::ate);

        if(!_iStream)
        {
            throw std::exception("Failed to load file");
        }

        std::streampos _end = _iStream.tellg();
        _iStream.seekg(0, std::ios::beg);
        uint32_t _size = _end - _iStream.tellg();

        if(_size == 0)
        {
            throw std::exception("File is empty");
        }

        char* _buffer = new char[_size];

        _iStream.read(_buffer, _size);
        _iStream.close();

        *_uiOutSize = _size;

        return _buffer;
    }

    MonoAssembly* CSharp::LoadAssembly(const char* _sFilePath)
    {
        uint32_t _size = 0;
        char* _fileData = ReadBytes(_sFilePath, &_size);

        MonoImageOpenStatus _status;
        MonoImage* _image = mono_image_open_from_data_full(_fileData, _size, true, &_status, false);

        if(_status != MONO_IMAGE_OK)
        {
            const char* _errorMessage = mono_image_strerror(_status);

            throw std::exception(_errorMessage);
        }

        uint32_t _pdbFileSize = 0;
        
        auto _pdbFileData = ReadBytes(std::filesystem::path(_sFilePath).replace_extension(".pdb").string().c_str(), &_pdbFileSize);

        mono_debug_open_image_from_memory(_image, reinterpret_cast<const mono_byte*>(_pdbFileData), _pdbFileSize);

        delete[] _pdbFileData;

        MonoAssembly* _assembly = mono_assembly_load_from_full(_image, _sFilePath, &_status, false);

        mono_image_close(_image);

        delete[] _fileData;

        return _assembly;
    }

    CSharp::CSharp(const Memory::FreeList& _heap, ECS::EntityFactory* _pEntityFactory, Duckvil::Event::Pool<Duckvil::Event::mode::immediate>* _pEngineEventPool) :
        m_heap(_heap),
        m_pEntityFactory(_pEntityFactory),
        m_pEngineEventPool(_pEngineEventPool)
    {
        gs_pScriptEngine = this;

        _pEngineEventPool->Add(Utils::lambda([&](InjectConstructorArgumentEvent& _e)
        {
            if(_e.m_bSuccess)
            {
                return;
            }
			
            static const size_t _typeID = typeid(Duckvil::CSharp::CSharp).hash_code();

            {
                if(_typeID == _e.m_argument.m_ullTypeID)
                {
                    _e.m_pFAP->Push(this);

                    _e.m_bSuccess = true;
                }
            }
        }));

        _pEngineEventPool->AddA<RequestSystemEvent>([&](RequestSystemEvent& _event)
        {
            if(_event.m_typeHandle.m_ID == RuntimeReflection::get_type<Duckvil::CSharp::CSharp>().m_ID)
            {
                _event.m_pRequestedSystem = this;
            }
        });

        m_csharpEventPool = Duckvil::Event::Pool<Duckvil::Event::mode::immediate>(_heap);

        m_csharpEventPool.Add(Utils::lambda([&](const Event::MonoObjectCreatedEvent& _event)
        {
            MonoClass* _sharedScriptClass = mono_class_from_name(mono_assembly_get_image(static_cast<MonoAssembly*>(m_pCoreAssembly)), "Duckvil", "SharedScript");

            if(!mono_class_implements_interface(static_cast<MonoClass*>(_event.m_pClass), _sharedScriptClass))
            {
                return;
            }

            const auto& _namespace = cpp_combine(static_cast<MonoClass*>(_event.m_pClass));
            std::vector<const char*> _namespaceCstr;

            for(const auto& _n : _namespace)
            {
                _namespaceCstr.push_back(_n.c_str());
            }

            auto _typeHandle = RuntimeReflection::get_type(mono_class_get_name(static_cast<MonoClass*>(_event.m_pClass)), _namespaceCstr);

            SharedScript* _nativeClass = Create(_typeHandle);
            size_t _id = -1;

            Register(&_id, _nativeClass);

            {
                auto _methodSharedScriptSetID = mono_class_get_method_from_name(static_cast<MonoClass*>(_event.m_pClass), "SetSharedScriptID", 1);

                void* _params[1];

                _params[0] = &_id;

                mono_runtime_invoke(_methodSharedScriptSetID, _event.m_pObject, _params, nullptr);
            }

            _nativeClass->SetCSharpClass(static_cast<MonoClass*>(_event.m_pClass));
            _nativeClass->SetCSharpObject(static_cast<MonoObject*>(_event.m_pObject));
            _nativeClass->InternalInit();
        }));
    }

    CSharp::~CSharp()
    {
        mono_domain_unload(m_pAppDomain);
        mono_jit_cleanup(m_pRootDomain);
    }

    bool CSharp::Setup()
    {
        ecs_os_set_api_defaults();

        mono_set_assemblies_path("C:/Program Files/Mono/lib");

        const char* _argv[2] =
        {
            "--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n",
            "--soft-breakpoints"
        };

        mono_jit_parse_options(2, (char**)_argv);
        mono_debug_init(MONO_DEBUG_FORMAT_MONO);

        m_pRootDomain = mono_jit_init(ms_sRootDomainName);

        if(!m_pRootDomain)
        {
            throw std::exception();
        }

        mono_debug_domain_create(m_pRootDomain);

        m_pAppDomain = mono_domain_create_appdomain((char*)ms_sAppDomainName, nullptr);

        mono_thread_set_main(mono_thread_current());

        mono_domain_set(m_pAppDomain, true);

        return true;
    }

    inline Utils::string cs_combine(const RuntimeReflection::__type_t& _type)
    {
        Utils::string _res("");

        for(uint32_t _i = 0; _i < DUCKVIL_SLOT_ARRAY_SIZE(_type.m_namespaces); _i++)
        {
            _res = _res + DUCKVIL_SLOT_ARRAY_GET(_type.m_namespaces, _i).m_sNamespaceName;

            if(_i < DUCKVIL_SLOT_ARRAY_SIZE(_type.m_namespaces) - 1)
            {
                _res = _res + ".";
            }
        }

        return _res;
    }

#define DUCKVIL_CS_COMPONENT(name) \
    { \
    MonoType* _managedType = mono_reflection_type_from_name((char*)"Duckvil." # name, _coreImage); \
 \
    Function _f; \
 \
    _f.m_fnHasComponent = [](ECS::Entity _entity) \
    { \
        return _entity.Has<name>(); \
    }; \
 \
    _f.m_fnAddComponent = [](ECS::Entity _entity, void* _pData) \
    { \
        if(_pData == nullptr) \
        { \
            _entity.Add<name>(); \
        } \
        else \
        { \
            name* _data = reinterpret_cast<name*>(static_cast<uint8_t*>(_pData) + sizeof(size_t)); \
 \
            _entity.Add<name>(*_data); \
        } \
 \
        return true; \
    }; \
 \
    m_aComponentFuncs[_managedType] = _f; \
    }

    using TransformComponent = Graphics::TransformComponent;

    bool CSharp::Init()
    {
        m_pCoreAssembly = LoadAssembly((std::filesystem::path(DUCKVIL_OUTPUT) / "Managed/Debug/net7.0/CSDuckvil.dll").string().c_str());
        m_pAppAssembly = LoadAssembly((std::filesystem::path(DUCKVIL_OUTPUT) / "Managed/Debug/net7.0/Sandbox.dll").string().c_str());

        MonoImage* _coreImage = mono_assembly_get_image(static_cast<MonoAssembly*>(m_pCoreAssembly));
        MonoImage* _appImage = mono_assembly_get_image(static_cast<MonoAssembly*>(m_pAppAssembly));

        DUCKVIL_CS_COMPONENT(TransformComponent);

        mono_add_internal_call("Duckvil.InternalCalls::Entity_HasComponent", entity_has_component);
        mono_add_internal_call("Duckvil.InternalCalls::Entity_AddComponent", entity_add_component);

        mono_add_internal_call("Duckvil.InternalCalls::TransformComponent_GetTranslation", transform_component_get_translation);
        mono_add_internal_call("Duckvil.InternalCalls::TransformComponent_SetTranslation", transform_component_set_translation);
        mono_add_internal_call("Duckvil.InternalCalls::TransformComponent_GetRotation", transform_component_get_rotation);
        mono_add_internal_call("Duckvil.InternalCalls::TransformComponent_SetRotation", transform_component_set_rotation);
        mono_add_internal_call("Duckvil.InternalCalls::TransformComponent_GetScale", transform_component_get_scale);
        mono_add_internal_call("Duckvil.InternalCalls::TransformComponent_SetScale", transform_component_set_scale);

        mono_add_internal_call("Duckvil.SharedScript::Register", shared_script_register);

        /*auto _th = RuntimeReflection::get_type<CSharp>();
        auto _ph = RuntimeReflection::get_property(_th, "ms_sRootDomainName");
        auto _p = RuntimeReflection::get_property(_th, _ph);

        auto _x = *reinterpret_cast<const char**>(_p.m_ullAddress);*/



        return true;
    }

    void CSharp::Update()
    {
        for(const std::pair<size_t, SharedScript*>& _sharedScript : m_aSharedScripts)
        {
            _sharedScript.second->Update();
        }
    }

    bool CSharp::Shutdown()
    {
        return true;
    }

    MonoObject* CSharp::Create(void* _pClass)
    {
        MonoObject* _object = mono_object_new(m_pAppDomain, static_cast<MonoClass*>(_pClass));

        m_csharpEventPool.Broadcast(Event::MonoObjectCreatedEvent{ _object, static_cast<MonoClass*>(_pClass) });

        return _object;
    }

    void* CSharp::GetType(const char* _sTypeName, const char** _saNamepsace, uint32_t _uiCount)
    {
        std::string _namespace;

        for(uint32_t _i = 0; _i < _uiCount; ++_i)
        {
            _namespace += _saNamepsace[_i];

            if(_i < _uiCount - 1)
            {
                _namespace += ".";
            }
        }

        return mono_class_from_name(mono_assembly_get_image(static_cast<MonoAssembly*>(m_pCoreAssembly)), _namespace.c_str(), _sTypeName);
    }

    void* CSharp::CreateObject(void* _pType)
    {
        return mono_object_new(m_pAppDomain, static_cast<MonoClass*>(_pType));
    }

}}