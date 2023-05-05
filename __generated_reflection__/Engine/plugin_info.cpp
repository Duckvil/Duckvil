#include "RuntimeReflection/Recorder.h"
#include "Logger/Logger.h"
DUCKVIL_RUNTIME_REFLECTION_RECORDERS_COUNT(16)

DUCKVIL_EXPORT void duckvil_plugin_make_current_runtime_reflection_context(const duckvil_frontend_reflection_context& _runtimeReflectionContext)
{
Duckvil::RuntimeReflection::make_current(_runtimeReflectionContext);
}

DUCKVIL_EXPORT void duckvil_plugin_make_current_logger_context(const Duckvil::logger_context& _loggerContext)
{
Duckvil::logger_make_current(_loggerContext);
}

DUCKVIL_EXPORT void duckvil_plugin_make_current_heap_context(const Duckvil::Memory::free_list_context& _heapContext)
{
Duckvil::Memory::heap_make_current(_heapContext);
}

