#include "RuntimeCompiler\RuntimeCompilerSystem.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(4)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
Duckvil::Memory::Vector<DUCKVIL_RESOURCE(type_t)> _types(_pMemoryInterface, _pAllocator, 1);
using namespace Duckvil::RuntimeCompiler;
_type = record_type<Duckvil::RuntimeCompiler::hot_object>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "hot_object");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
_property = record_property<void**>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::hot_object, m_pObject), "m_pObject");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::hot_object, m_typeHandle), "m_typeHandle");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
_type = record_type<Duckvil::RuntimeCompiler::RuntimeCompilerSystem>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "RuntimeCompilerSystem");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, ReflectionFlags::ReflectionFlags_EngineSystem, true);
record_inheritance(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, get_type<ISystem>(_pData), __protection::__protection_public);
_constructor = record_constructor<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, const Memory::FreeList&, RuntimeReflection::__data*, RuntimeReflection::__recorder_ftable*, RuntimeReflection::__ftable*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<RuntimeReflection::__data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem, m_pReflectionData), "m_pReflectionData");
_property = record_property<Memory::Vector<hot_object>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem, m_aHotObjects), "m_aHotObjects");
_property = record_property<Memory::Vector<RuntimeCompilerSystem::reflection_module>>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem, m_aModules), "m_aModules");
record_function<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::RuntimeCompilerSystem::Init, "Init");
record_function<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::RuntimeCompilerSystem::Update, "Update");
record_function<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, void, const std::string&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::RuntimeCompilerSystem::Compile, "Compile");
record_function<Duckvil::RuntimeCompiler::RuntimeCompilerSystem, void, void**, RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::RuntimeCompiler::RuntimeCompilerSystem::AddHotObject, "AddHotObject");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
_type = record_type<Duckvil::RuntimeCompiler::RuntimeCompilerSystem::user_data>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "user_data");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
_property = record_property<RuntimeCompilerSystem*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::user_data, m_pRuntimeCompiler), "m_pRuntimeCompiler");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
_type = record_type<Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "reflection_module");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "RuntimeCompiler");
_property = record_property<void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_pObject), "m_pObject");
_property = record_property<RuntimeReflection::__duckvil_resource_type_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_typeHandle), "m_typeHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_generateCustomFunctionHandle), "m_generateCustomFunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_processAST_FunctionHandle), "m_processAST_FunctionHandle");
_property = record_property<RuntimeReflection::__duckvil_resource_function_t>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::RuntimeCompiler::RuntimeCompilerSystem::reflection_module, m_clearFunctionHandle), "m_clearFunctionHandle");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
return _types;
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 4;
}
#endif
