#include "Window/SDL_Window.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(8)
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
_namespaces.push_back("WindowSDL");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Window;
_type = record_type<Duckvil::Window::WindowSDL>(_data, "WindowSDL");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Window");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 8);
record_inheritance<IWindow>(_data, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Window::WindowSDL, Event::Pool<Event::mode::buffered>*>(_data, _type);
_destructor = record_destructor<Duckvil::Window::WindowSDL>(_data, _type);
_function = record_function<Duckvil::Window::WindowSDL, bool, char const*, int, int>(_data, _type, &Duckvil::Window::WindowSDL::Create, "Create");
_function = record_function<Duckvil::Window::WindowSDL, void>(_data, _type, &Duckvil::Window::WindowSDL::Refresh, "Refresh");
_function = record_function<Duckvil::Window::WindowSDL, void>(_data, _type, &Duckvil::Window::WindowSDL::PopulateEvents, "PopulateEvents");
_function = record_function<Duckvil::Window::WindowSDL, void*>(_data, _type, &Duckvil::Window::WindowSDL::GetWindow, "GetWindow");
_function = record_function<Duckvil::Window::WindowSDL, void*>(_data, _type, &Duckvil::Window::WindowSDL::GetContext, "GetContext");
_function = record_function<Duckvil::Window::WindowSDL, void, int, int, bool>(_data, _type, &Duckvil::Window::WindowSDL::SetMousePosition, "SetMousePosition");
_function = record_function<Duckvil::Window::WindowSDL, void, void(*)(void*)>(_data, _type, &Duckvil::Window::WindowSDL::SetProcessEventsCallback, "SetProcessEventsCallback");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Window/SDL_Window.h", 8);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 8;
}
#endif
