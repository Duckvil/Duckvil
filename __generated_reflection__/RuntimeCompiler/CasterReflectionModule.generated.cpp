#include "RuntimeCompiler\CasterReflectionModule.h"
#include "RuntimeReflection/Recorder.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
Duckvil::Memory::Vector<DUCKVIL_RESOURCE(type_t)> _types(_pMemoryInterface, _pAllocator, 1);
using namespace Duckvil;
_type = record_type<Duckvil::CasterReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "CasterReflectionModule");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
_type = record_type<Duckvil::CasterReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "CasterReflectionModule");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, ReflectionFlags::ReflectionFlags_ReflectionModule, true);
_constructor = record_constructor<Duckvil::CasterReflectionModule>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_property = record_property<bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, offsetof(Duckvil::CasterReflectionModule, m_bHasGeneratedBody), "m_bHasGeneratedBody");
record_function<Duckvil::CasterReflectionModule, void, Parser::__ast*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::CasterReflectionModule::ProcessAST, "ProcessAST");
record_function<Duckvil::CasterReflectionModule, void, std::ofstream&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::CasterReflectionModule::GenerateCustom, "GenerateCustom");
record_function<Duckvil::CasterReflectionModule, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::CasterReflectionModule::Clear, "Clear");
if(_types.Full()) _types.Resize(_types.Size() * 2);
_types.Allocate(_type);
return _types;
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
