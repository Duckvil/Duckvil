#include "Window/IWindow.h"
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
_namespaces.push_back("Window");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("IWindow");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Window;
_type = record_type<Duckvil::Window::IWindow>(_data, "IWindow");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Window");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
_function = record_function<Duckvil::Window::IWindow, bool, char const*, uint32_t, uint32_t>(_data, _type, &Duckvil::Window::IWindow::Create, "Create");
_function = record_function<Duckvil::Window::IWindow, void>(_data, _type, &Duckvil::Window::IWindow::Refresh, "Refresh");
_function = record_function<Duckvil::Window::IWindow, void*>(_data, _type, &Duckvil::Window::IWindow::GetWindow, "GetWindow");
_function = record_function<Duckvil::Window::IWindow, void*>(_data, _type, &Duckvil::Window::IWindow::GetContext, "GetContext");
_function = record_function<Duckvil::Window::IWindow, void>(_data, _type, &Duckvil::Window::IWindow::PopulateEvents, "PopulateEvents");
_function = record_function<Duckvil::Window::IWindow, void, int, int, bool>(_data, _type, &Duckvil::Window::IWindow::SetMousePosition, "SetMousePosition");
_function = record_function<Duckvil::Window::IWindow, void, void(*)(void*)>(_data, _type, &Duckvil::Window::IWindow::SetProcessEventsCallback, "SetProcessEventsCallback");
_function = record_function<Duckvil::Window::IWindow, uint32_t>(_data, _type, &Duckvil::Window::IWindow::GetWidth, "GetWidth");
_function = record_function<Duckvil::Window::IWindow, uint32_t>(_data, _type, &Duckvil::Window::IWindow::GetHeight, "GetHeight");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Window/IWindow.h", 6);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 6;
}
#endif
