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
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Window;
_type = record_type<Duckvil::Window::IWindow>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "IWindow");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Window");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 6);
record_function<Duckvil::Window::IWindow, bool, const char*, int, int>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Window::IWindow::Create, "Create");
record_function<Duckvil::Window::IWindow, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Window::IWindow::Refresh, "Refresh");
record_function<Duckvil::Window::IWindow, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Window::IWindow::GetWindow, "GetWindow");
record_function<Duckvil::Window::IWindow, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Window::IWindow::GetContext, "GetContext");
record_function<Duckvil::Window::IWindow, void, int, int>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Window::IWindow::SetMousePosition, "SetMousePosition");
record_function<Duckvil::Window::IWindow, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Window::IWindow::SetProcessEventsCallback, "SetProcessEventsCallback");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Window/IWindow.h", 6);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 6;
}
#endif
