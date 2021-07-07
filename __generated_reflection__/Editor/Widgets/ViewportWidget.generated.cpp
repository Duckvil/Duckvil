#include "Editor/Widgets/ViewportWidget.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"

DUCKVIL_RUNTIME_REFLECTION_RECORD(13)
{
using namespace Duckvil::RuntimeReflection;
using namespace Duckvil;
DUCKVIL_RESOURCE(type_t) _type;
DUCKVIL_RESOURCE(property_t) _property;
DUCKVIL_RESOURCE(constructor_t) _constructor;
DUCKVIL_RESOURCE(destructor_t) _destructor;
std::vector<Duckvil::RuntimeReflection::__duckvil_resource_type_t> _recordedTypes;
using namespace Duckvil::Editor;
_type = record_type<Duckvil::Editor::ViewportWidget>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, "ViewportWidget");
_recordedTypes.push_back(_type);
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Duckvil");
record_namespace(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, "Editor");
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 13);
record_meta(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, Duckvil::ReflectionFlags::ReflectionFlags_UserSystem, true);
record_inheritance<Widget>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
record_inheritance<ISystem>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, __protection::__protection_public);
_constructor = record_constructor<Duckvil::Editor::ViewportWidget>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_constructor = record_constructor<Duckvil::Editor::ViewportWidget, const Memory::FreeList&>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
_destructor = record_destructor<Duckvil::Editor::ViewportWidget>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type);
record_function<Duckvil::Editor::ViewportWidget, bool>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::Init, "Init");
record_function<Duckvil::Editor::ViewportWidget, void, double>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::Update, "Update");
record_function<Duckvil::Editor::ViewportWidget, void, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::InitEditor, "InitEditor");
record_function<Duckvil::Editor::ViewportWidget, void>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::OnDraw, "OnDraw");
record_function<Duckvil::Editor::ViewportWidget, void, Graphics::Renderer::renderer_ftable*, Graphics::Renderer::renderer_data*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::SetRenderer, "SetRenderer");
record_function<Duckvil::Editor::ViewportWidget, void, Event::Pool<Event::mode::buffered>*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::SetEventPool, "SetEventPool");
record_function<Duckvil::Editor::ViewportWidget, void, flecs::world*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::SetECS, "SetECS");
record_function<void*, void*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::Cast, "Cast");
record_function<Duckvil::Editor::ViewportWidget, void, Duckvil::RuntimeSerializer::ISerializer*>(DUCKVIL_RUNTIME_REFLECTION_RECORDER_STANDARD_STUFF, _type, &Duckvil::Editor::ViewportWidget::Serialize, "Serialize");
return duckvil_recorded_types_create(_pMemoryInterface, _pAllocator, _recordedTypes, "Editor/Widgets/ViewportWidget.h", 13);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 13;
}
#endif
