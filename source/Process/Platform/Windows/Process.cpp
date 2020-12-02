#include "Process/Platform/Windows/Process.h"

#include "Process/Process.h"

#include <process.h>
#include <Windows.h>

namespace Duckvil { namespace Process {

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
        windows_data* pImpl = (windows_data*)arg;

        while(_readActive && ((data*)pImpl->m_pData)->m_bRunning)
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
                    ((data*)pImpl->m_pData)->m_bComplete = true;
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

    void windows_init(Duckvil::Memory::IMemory* _pMemory, Duckvil::Memory::__free_list_allocator* _pAllocator, void** _pImplementationData)
    {
        windows_data* _data = (windows_data*)_pMemory->m_fnFreeListAllocate_(_pAllocator, sizeof(windows_data), alignof(windows_data));

        *_pImplementationData = _data;
    }

    void windows_write(void* _pImplementationData, const char* _csMessage)
    {
        if(!((data*)((windows_data*)_pImplementationData)->m_pData)->m_bRunning)
        {
            return;
        }

        DWORD nBytesWritten;
        DWORD length = (DWORD)strlen(_csMessage);
        windows_data* _data = (windows_data*)_pImplementationData;

        ((data*)_data->m_pData)->m_bComplete = false;

        WriteFile(
            _data->m_pCommandProcessInputWrite,
            _csMessage,
            length,
            &nBytesWritten,
            NULL
        );
    }

    bool windows_setup(void* _pData, void* _pImplementationData)
    {
        ((windows_data*)_pImplementationData)->m_pData = _pData;

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
                &(((windows_data*)_pImplementationData)->m_pCommandProcessOutputRead),
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
                &(((windows_data*)_pImplementationData)->m_pCommandProcessInputWrite),
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
            &((windows_data*)_pImplementationData)->m_commandProcessInfo
        );

        // WriteInput("vcvarsall x86_amd64\n");

        return true;
    }

    void windows_start(void* _pImplementationData)
    {
        ((data*)((windows_data*)_pImplementationData)->m_pData)->m_bRunning = true;
        _beginthread(readAndHandleOutputThread, 0, _pImplementationData);
    }

    void windows_stop(void* _pImplementationData)
    {
        ((data*)((windows_data*)_pImplementationData)->m_pData)->m_bRunning = false;
        // TODO: Something else?
    }

}}