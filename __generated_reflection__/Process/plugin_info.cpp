#include "RuntimeReflection/Recorder.h"
#include "Logger/LoggerChannel.h"
DUCKVIL_RUNTIME_REFLECTION_RECORD_COUNT(3)

DUCKVIL_EXPORT void duckvil_plugin_make_current_runtime_reflection_context(const duckvil_frontend_reflection_context& _runtimeReflectionContext)
{
Duckvil::RuntimeReflection::make_current(_runtimeReflectionContext);
}

DUCKVIL_EXPORT void duckvil_plugin_make_current_logger_context(const Duckvil::logger_context& _loggerContext)
{
Duckvil::logger_make_current(_loggerContext);
}