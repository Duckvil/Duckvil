#include "Process/Platform/Linux/Process.h"

#ifdef DUCKVIL_PLATFORM_LINUX
#include <sys/wait.h>
#include <unistd.h>
#endif

#include "Memory/FreeList.h"

namespace Duckvil { namespace Process {

#ifdef DUCKVIL_PLATFORM_LINUX
    struct linux_data
    {
        pid_t m_pid;
        void* m_pData;
    };

    void linux_init(Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData)
    {
        linux_data* _data = static_cast<linux_data*>(_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(linux_data), alignof(linux_data)));

        _pData->m_pImplementationData = _data;
    }

    bool linux_setup(data* _pData)
    {
        return true;
    }

    void linux_write(data* _pData, const char* _csMessage)
    {
        linux_data* _data = (linux_data*)_pData->m_pImplementationData;

        _data->m_pid = fork();

    // TODO: It is currently blocking the parent process

        switch(_data->m_pid)
        {
        case -1:
            break;
        case 0:
            execl("/bin/sh", "/bin/sh", "-c", _csMessage, nullptr);
            break;
        default:
            int _status;
            waitpid(_data->m_pid, &_status, 0);
            break;
        }
    }

    void linux_start(data* _pData)
    {

    }

    void linux_stop(data* _pData)
    {

    }

    void linux_wait(data* _pData)
    {
        // wait(nullptr);
    }

    bool linux_terminate(data* _pData)
    {
        throw "Not implemented yet!";

        return true;
    }

    void linux_cleanup(Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData)
    {
        Memory::free_list_free(_pMemory, _pAllocator, _pData->m_pImplementationData);
    }
#endif

}}