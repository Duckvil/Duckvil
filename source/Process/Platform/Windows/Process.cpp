#include "Process/Platform/Windows/Process.h"

#include "Process/Process.h"

#include "Memory/FreeList.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <process.h>
#include <Windows.h>
#endif

namespace Duckvil { namespace Process {

#ifdef DUCKVIL_PLATFORM_WINDOWS
    struct windows_data
    {
        HANDLE m_pCommandProcessOutputRead;
        HANDLE m_pCommandProcessInputWrite;
        PROCESS_INFORMATION m_commandProcessInfo;
        void* m_pData;
    };

    void readAndHandleOutputThread(LPVOID arg)
    {
        CHAR lpBuffer[1024];
        DWORD nBytesRead;
        bool _readActive = true;
        bool _readOneMore = false;
        windows_data* pImpl = static_cast<windows_data*>(arg);

        while(_readActive && static_cast<data*>(pImpl->m_pData)->m_bRunning)
        {
            if(!ReadFile(
                pImpl->m_pCommandProcessOutputRead,
                lpBuffer,
                sizeof(lpBuffer) - 1,
                &nBytesRead,
                nullptr))
            {
                _readActive = false;

                if(GetLastError() != ERROR_BROKEN_PIPE)
                {
                    printf("Runtime Compiler: Redirect of compile output failed on read\n");
                }
            }
            else
            {
                lpBuffer[nBytesRead] = 0;

                std::string buffer(lpBuffer);
                size_t found = buffer.find(cs_CompletionToken);

                if(found != std::string::npos)
                {
                    buffer = buffer.substr(0, found);
                    static_cast<data*>(pImpl->m_pData)->m_bComplete = true;
                }

                if(_readActive || buffer.length())
                {
                    if(buffer.size() > 0 && buffer[buffer.size() - 1] == '\n')
                    {
                        buffer[buffer.size() - 1] = '\0';
                    }

                    bool wasError = buffer.find("error") != std::string::npos;
                    bool wasFatal = buffer.find("fatal") != std::string::npos;
                    bool wasWarning = buffer.find("warning") != std::string::npos;
                    bool wasNote = buffer.find("note") != std::string::npos;

                    if(wasWarning || wasNote)
                    {
                        continue;
                    }
                    else
                    {
                        if(wasFatal || wasError)
                        {
                            // pImpl->m_ErrorMessage.append(buffer + "\n");
                            // pImpl->m_Error = true;

                            printf("%s\n", buffer.c_str());
                        }
                        else
                        {
                            printf("%s", buffer.c_str());
                        }
                    }
                }
            }
        }
    }

    void windows_init(Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData)
    {
        // windows_data* _data = (windows_data*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(windows_data), alignof(windows_data));
        windows_data* _data = Memory::free_list_allocate<windows_data>(_pMemory, _pAllocator, windows_data{});

        _pData->m_pImplementationData = _data;
    }

    void windows_write(data* _pData, const char* _csMessage)
    {
        if(!_pData->m_bRunning)
        {
            return;
        }

        DWORD nBytesWritten;
        DWORD length = (DWORD)strlen(_csMessage);
        windows_data* _data = static_cast<windows_data*>(_pData->m_pImplementationData);

        _pData->m_bComplete = false;

        WriteFile(
            _data->m_pCommandProcessInputWrite,
            _csMessage,
            length,
            &nBytesWritten,
            NULL
        );
    }

    bool windows_setup(data* _pData)
    {
        static_cast<windows_data*>(_pData->m_pImplementationData)->m_pData = _pData;
        _pData->m_bRunning = true;

        STARTUPINFOW _si;

        ZeroMemory(&_si, sizeof(STARTUPINFO));

        _si.cb = sizeof(STARTUPINFO);
        _si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        _si.wShowWindow = SW_HIDE;

        HANDLE _outputReadTmp = nullptr;
        HANDLE _outputWrite = nullptr;
        HANDLE _errorWrite = nullptr;

        SECURITY_ATTRIBUTES _sa;

        _sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        _sa.lpSecurityDescriptor = nullptr;
        _sa.bInheritHandle = TRUE;

        if(!CreatePipe(
            &_outputReadTmp,
            &_outputWrite,
            &_sa,
            20 * 1024))
        {
            return false;
        }

        _si.hStdOutput = _outputWrite;

        if(!DuplicateHandle(
            GetCurrentProcess(),
            _outputWrite,
            GetCurrentProcess(),
            &_errorWrite,
            0,
            TRUE,
            DUPLICATE_SAME_ACCESS))
        {
            return false;
        }

        _si.hStdError = _errorWrite;

        if(_si.hStdOutput)
        {
            if(!DuplicateHandle(
                GetCurrentProcess(),
                _outputReadTmp,
                GetCurrentProcess(),
                &(static_cast<windows_data*>(_pData->m_pImplementationData)->m_pCommandProcessOutputRead),
                0,
                FALSE,
                DUPLICATE_SAME_ACCESS))
            {
                return false;
            }

            CloseHandle(_outputReadTmp);

            _outputReadTmp = nullptr;
        }

        HANDLE _inputRead = nullptr;
        HANDLE _inputWriteTmp = nullptr;

        if(!CreatePipe(
            &_inputRead,
            &_inputWriteTmp,
            &_sa,
            4096))
        {
            return false;
        }

        _si.hStdInput = _inputRead;

        if(_si.hStdOutput)
        {
            if(!DuplicateHandle(
                GetCurrentProcess(),
                _inputWriteTmp,
                GetCurrentProcess(),
                &(static_cast<windows_data*>(_pData->m_pImplementationData)->m_pCommandProcessInputWrite),
                0,
                FALSE,
                DUPLICATE_SAME_ACCESS))
            {
                return false;
            }
        }

        const wchar_t* _commandLine = L"cmd /q";
        wchar_t _commandLineNonConst[1024];

        wcscpy_s(_commandLineNonConst, _commandLine);

        CreateProcessW(
            nullptr,
            _commandLineNonConst,
            nullptr,
            nullptr,
            TRUE,
            0,
            nullptr,
            nullptr,
            &_si,
            &(static_cast<windows_data*>(_pData->m_pImplementationData)->m_commandProcessInfo)
        );

        windows_write(_pData, ("\"" + std::string(DUCKVIL_VCVARS_PATH) + "\" x86_amd64\n").c_str());
        _beginthread(readAndHandleOutputThread, 0, _pData->m_pImplementationData);

        return true;
    }

    void windows_start(data* _pData)
    {
        _pData->m_bRunning = true;

        _beginthread(readAndHandleOutputThread, 0, _pData->m_pImplementationData);
    }

    void windows_stop(data* _pData)
    {
        _pData->m_bRunning = false;
    }

    void windows_wait(data* _pData)
    {
        while(!_pData->m_bComplete)
        {

        }
    }

    bool windows_terminate(data* _pData)
    {
        windows_data* _data = static_cast<windows_data*>(_pData->m_pImplementationData);

        if(!TerminateProcess(_data->m_commandProcessInfo.hProcess, 0))
        {
            return false;
        }

        return true;
    }

    void windows_cleanup(Duckvil::Memory::ftable* _pMemory, Duckvil::Memory::free_list_allocator* _pAllocator, data* _pData)
    {
        Memory::free_list_free(_pMemory, _pAllocator, _pData->m_pImplementationData);
    }
#endif

}}