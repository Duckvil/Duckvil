#include "Editor/Widgets/NewProjectWidget.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(15)
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
_namespaces.push_back("Editor");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("NewProjectWidget");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::NewProjectWidget>(_data, "NewProjectWidget");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Editor");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 15);
record_meta(_data, _type, ReflectionFlag::Spwanable, true);
record_inheritance<Widget>(_data, _type, __protection::__protection_public);
record_inheritance<ISystem>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Editor::NewProjectWidget>(_data, _type);
_constructor = record_constructor<Duckvil::Editor::NewProjectWidget, const Memory::FreeList&, const Event::Pool<Event::mode::immediate>*, ProjectManager::ftable*, ProjectManager::data*>(_data, _type);
record_meta(_data, _type, _constructor, 1, "Editor", true);
_destructor = record_destructor<Duckvil::Editor::NewProjectWidget>(_data, _type);
_function = record_function<Duckvil::Editor::NewProjectWidget, void, void*>(_data, _type, &Duckvil::Editor::NewProjectWidget::InitEditor, "InitEditor");
_function = record_function<Duckvil::Editor::NewProjectWidget, void>(_data, _type, &Duckvil::Editor::NewProjectWidget::OnDraw, "OnDraw");
_function = record_function<Duckvil::Editor::NewProjectWidget, bool>(_data, _type, &Duckvil::Editor::NewProjectWidget::Init, "Init");
_function = record_function<Duckvil::Editor::NewProjectWidget, void, double>(_data, _type, &Duckvil::Editor::NewProjectWidget::Update, "Update");
_function = record_function<void*, void*>(_data, _type, &Duckvil::Editor::NewProjectWidget::Cast, "Cast");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_function = record_function<Duckvil::Editor::NewProjectWidget, void, Duckvil::RuntimeSerializer::ISerializer*>(_data, _type, &Duckvil::Editor::NewProjectWidget::Serialize, "Serialize");
record_meta(_data, _type, _function, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_AutoGenerated, true);
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Editor/Widgets/NewProjectWidget.h", 15);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 15;
}
#endif
