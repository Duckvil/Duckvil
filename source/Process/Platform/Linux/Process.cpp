#include "Process/Platform/Linux/Process.h"

#ifdef DUCKVIL_PLATFORM_LINUX
#include <unistd.h>
#endif

namespace Duckvil { namespace Process {

#ifdef DUCKVIL_PLATFORM_LINUX
    struct linux_data
    {
        pid_t m_pid;
        void* m_pData;
    };

    void linux_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, data* _pData)
    {
        linux_data* _data = (linux_data*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(linux_data), alignof(linux_data));

        _pData->m_pImplementationData = _data;
    }

    bool linux_setup(data* _pData)
    {
        // linux_data* _data = (linux_data*)_pData->m_pImplementationData;

        // _data->m_pid = fork();

        return true;
    }

    void linux_write(data* _pData, const char* _csMessage)
    {
        linux_data* _data = (linux_data*)_pData->m_pImplementationData;

        _data->m_pid = fork();

        if(_data->m_pid == 0)
        {
            execl(_csMessage, _csMessage, nullptr);
        }
    }

    void linux_start(data* _pData)
    {

    }

    void linux_stop(data* _pData)
    {

    }
#endif

}}