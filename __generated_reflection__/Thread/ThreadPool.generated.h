#include "Serializer/Runtime/ISerializer.h"

#define DUCKVIL_GENERATED_BODY \
public: \
static void* Cast(void* _pObject) { return static_cast<pool_ftable*>(_pObject); } \
public: \
void Serialize(Duckvil::RuntimeSerializer::ISerializer* _pSerializer) \
{\
_pSerializer->SerializeProperty("m_uiThreadsCount", m_uiThreadsCount); \
_pSerializer->SerializeProperty("m_aWorkers", m_aWorkers); \
_pSerializer->SerializeProperty("m_heap", m_heap); \
_pSerializer->SerializeProperty("m_lock", m_lock); \
_pSerializer->SerializeProperty("m_threadPoolLock", m_threadPoolLock); \
_pSerializer->SerializeProperty("m_condition", m_condition); \
_pSerializer->SerializeProperty("m_aTasks", m_aTasks); \
_pSerializer->SerializeProperty("m_bRunning", m_bRunning); \
_pSerializer->SerializeProperty("m_bTerminate", m_bTerminate); \
}
