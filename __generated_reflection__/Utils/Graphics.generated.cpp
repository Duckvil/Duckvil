#include "Utils/Graphics.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(6)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
DUCKVIL_RESOURCE(function_t) _function;
DUCKVIL_RESOURCE(enum_t) _enum;
DUCKVIL_RESOURCE(enum_element_t) _enumElement;
DUCKVIL_RESOURCE(ntype_t) _ntype;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_ntype_t> _recordedNTypes;
std::vector<const char*> _namespaces;
{
_namespaces.push_back("Duckvil");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Utils");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
{
_namespaces.push_back("Graphics");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("FrameBufferReader");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Utils::Graphics;
_type = record_type<Duckvil::Utils::Graphics::FrameBufferReader>(_data, "FrameBufferReader");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Utils");
record_namespace(_data, _type, "Graphics");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
_constructor = record_constructor<Duckvil::Utils::Graphics::FrameBufferReader>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::Graphics::FrameBufferReader, Memory::ftable*, Memory::free_list_allocator*, uint32_t, uint32_t, Duckvil::Utils::Graphics::FrameBufferReader::Format, Duckvil::Utils::Graphics::FrameBufferReader::Type>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::Graphics::FrameBufferReader, Duckvil::Utils::Graphics::FrameBufferReader const&>(_data, _type);
_constructor = record_constructor<Duckvil::Utils::Graphics::FrameBufferReader, Duckvil::Utils::Graphics::FrameBufferReader&&>(_data, _type);
_destructor = record_destructor<Duckvil::Utils::Graphics::FrameBufferReader>(_data, _type);
_function = record_function<Duckvil::Utils::Graphics::FrameBufferReader, Duckvil::Utils::Graphics::FrameBufferReader&, Duckvil::Utils::Graphics::FrameBufferReader const&>(_data, _type, &Duckvil::Utils::Graphics::FrameBufferReader::operator=, "operator=");
_function = record_function<Duckvil::Utils::Graphics::FrameBufferReader, Duckvil::Utils::Graphics::FrameBufferReader&, Duckvil::Utils::Graphics::FrameBufferReader&&>(_data, _type, &Duckvil::Utils::Graphics::FrameBufferReader::operator=, "operator=");
_function = record_function<Duckvil::Utils::Graphics::FrameBufferReader, void, void(void*,uint32_t,uint32_t,uint32_t)>(_data, _type, &Duckvil::Utils::Graphics::FrameBufferReader::Update, "Update");
_function = record_function<Duckvil::Utils::Graphics::FrameBufferReader, void, uint32_t, uint32_t, uint32_t, uint8_t>(_data, _type, &Duckvil::Utils::Graphics::FrameBufferReader::Read, "Read");
_function = record_function<Duckvil::Utils::Graphics::FrameBufferReader, void>(_data, _type, &Duckvil::Utils::Graphics::FrameBufferReader::Dispose, "Dispose");
_namespaces.pop_back();
_enum = record_enum<Duckvil::Utils::Graphics::FrameBufferReader::Format>(_data, _ntype, "Format");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Utils::Graphics::FrameBufferReader::Format::RED_INTEGER, "RED_INTEGER");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Utils::Graphics::FrameBufferReader::Format::RGBA, "RGBA");
_enum = record_enum<Duckvil::Utils::Graphics::FrameBufferReader::Type>(_data, _ntype, "Type");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Utils::Graphics::FrameBufferReader::Type::UNSIGNED_INT, "UNSIGNED_INT");
_enumElement = record_enum_element(_data, _ntype, _enum, Duckvil::Utils::Graphics::FrameBufferReader::Type::UNSIGNED_BYTE, "UNSIGNED_BYTE");
_namespaces.pop_back();
}
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Utils/Graphics.h", 6);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 6;
}
#endif
