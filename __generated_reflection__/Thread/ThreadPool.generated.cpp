#include "Thread/ThreadPool.h"
#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/GeneratedMeta.h"


DUCKVIL_RUNTIME_REFLECTION_RECORD(0)
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
_namespaces.push_back("Thread");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_namespaces.push_back("task");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
using namespace Duckvil::Thread;
_type = record_type<Duckvil::Thread::task>(_data, "task");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Thread");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<TaskCallback>(_data, _type, offsetof(Duckvil::Thread::task, m_callback), "m_callback");
_property = record_property<void*>(_data, _type, offsetof(Duckvil::Thread::task, m_pData), "m_pData");
_namespaces.pop_back();
_namespaces.push_back("pool_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
#ifdef DUCKVIL_PLATFORM_LINUX
_type = record_type<Duckvil::Thread::pool_data>(_data, "pool_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Thread");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_uiThreadsCount), "m_uiThreadsCount");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_bRunning), "m_bRunning");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_bTerminate), "m_bTerminate");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<pthread_mutex_t>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_lock), "m_lock");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<pthread_mutex_t>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_threadPoolLock), "m_threadPoolLock");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<pthread_cond_t>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_condition), "m_condition");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<Memory::Queue<task>>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_aTasks), "m_aTasks");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<Memory::Vector<pthread_t>>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_aWorkers), "m_aWorkers");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_heap), "m_heap");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<std::atomic<uint32_t>>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_uiTaskCount), "m_uiTaskCount");
#endif
_namespaces.pop_back();
_namespaces.push_back("pool_data");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
#ifdef DUCKVIL_PLATFORM_LINUX
_type = record_type<Duckvil::Thread::pool_data>(_data, "pool_data");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Thread");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<Memory::Vector<std::thread*>>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_aWorkers), "m_aWorkers");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<Memory::Queue<task>>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_aTasks), "m_aTasks");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<Memory::FreeList>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_heap), "m_heap");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<uint32_t>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_uiThreadsCount), "m_uiThreadsCount");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<std::condition_variable_any>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_condition), "m_condition");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<std::atomic<uint32_t>>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_uiTaskCount), "m_uiTaskCount");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_bRunning), "m_bRunning");
#endif
#ifdef DUCKVIL_PLATFORM_LINUX
_property = record_property<bool>(_data, _type, offsetof(Duckvil::Thread::pool_data, m_bTerminate), "m_bTerminate");
#endif
_namespaces.pop_back();
_namespaces.push_back("pool_ftable");
_ntype = record_type(_data, _namespaces);
_recordedNTypes.push_back(_ntype);
_type = record_type<Duckvil::Thread::pool_ftable>(_data, "pool_ftable");
_recordedTypes.push_back(_type);
record_namespace(_data, _type, "Duckvil");
record_namespace(_data, _type, "Thread");
record_meta(_data, _type, Duckvil::RuntimeReflection::GeneratedMeta::GeneratedMeta_RecorderID, 0);
_property = record_property<pool_data*(*)(const Memory::FreeList&)>(_data, _type, offsetof(Duckvil::Thread::pool_ftable, m_fnInit), "m_fnInit");
_property = record_property<void(*)(pool_data*)>(_data, _type, offsetof(Duckvil::Thread::pool_ftable, m_fnStart), "m_fnStart");
_property = record_property<void(*)(pool_data*)>(_data, _type, offsetof(Duckvil::Thread::pool_ftable, m_fnTerminate), "m_fnTerminate");
_property = record_property<void(*)(pool_data*, TaskCallback)>(_data, _type, offsetof(Duckvil::Thread::pool_ftable, m_fnOrderTask), "m_fnOrderTask");
_property = record_property<void(*)(pool_data*, TaskCallback, void*)>(_data, _type, offsetof(Duckvil::Thread::pool_ftable, m_fnOrderDataTask), "m_fnOrderDataTask");
_property = record_property<bool(*)(pool_data*)>(_data, _type, offsetof(Duckvil::Thread::pool_ftable, m_fnRemainingTasks), "m_fnRemainingTasks");
_property = record_property<uint32_t(*)(pool_data*)>(_data, _type, offsetof(Duckvil::Thread::pool_ftable, m_fnGetTaskCount), "m_fnGetTaskCount");
_namespaces.pop_back();
_namespaces.pop_back();
}
_namespaces.pop_back();
}
return duckvil_recorded_types_create(_data._pMemoryInterface, _data._pAllocator, _recordedTypes, "Thread/ThreadPool.h", 0);
}
#ifdef DUCKVIL_RUNTIME_COMPILE
DUCKVIL_EXPORT uint32_t duckvil_get_recorder_index()
{
return 0;
}
#endif
