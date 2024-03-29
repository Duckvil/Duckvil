#define STB_IMAGE_IMPLEMENTATION
#include <cstdio>
#include <filesystem>

#include "Utils/CommandArgumentsParser.h"
#include "Utils/Macro.h"

#include "PlugNPlay/Module.h"
#include "PlugNPlay/AutoLoader.h"
#include "PlugNPlay/Plugin.h"

#include "Memory/Vector.h"
#include "Memory/MemoryDebugger.h"

#include "RuntimeReflection/Recorder.h"
#include "RuntimeReflection/Generator.h"

#include "Parser/AST.h"

#include "Utils/Benchmark.h"

#include "Engine/Duckvil.h"

#include "Event/ImmediateChannel.h"

#ifdef DUCKVIL_PLATFORM_WINDOWS
#include <Windows.h>

VOID startup(LPSTR _cmd, LPCTSTR lpApplicationName)
{
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcess(lpApplicationName,   // the path
        _cmd,           // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );
    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

#endif

enum class Options
{
    PROJECT,
    SERVER,
    CLIENT,
    PORT,
    IP,
    TRACY
};

Duckvil::Utils::CommandArgumentsParser::Descriptor g_pDescriptors[] =
{
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::PROJECT, "project"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::SERVER, "server"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::CLIENT, "client"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::PORT, "port"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::IP, "ip"),
    Duckvil::Utils::CommandArgumentsParser::Descriptor(Options::TRACY, "tracy")
};

int main(int argc, char* argv[])
{
    Duckvil::Utils::CommandArgumentsParser _parser(argc, argv);

    if(!_parser.Parse(g_pDescriptors, DUCKVIL_ARRAY_SIZE(g_pDescriptors)))
    {
        return 1;
    }

    Duckvil::PlugNPlay::__module _module;
    Duckvil::PlugNPlay::__module_information _memoryModule("Memory");
    Duckvil::PlugNPlay::__module_information _engineModule("Engine");

    Duckvil::PlugNPlay::module_init(&_module);

    _module.load(&_memoryModule);
    _module.load(&_engineModule);

    duckvil_memory_init_callback duckvil_memory_init;
    Duckvil::__ftable* (*duckvil_init)();

    _module.get(_memoryModule, "duckvil_memory_init", reinterpret_cast<void**>(&duckvil_memory_init));
    _module.get(_engineModule, "duckvil_init", reinterpret_cast<void**>(&duckvil_init));

    const Duckvil::Memory::ftable* _memoryInterface = duckvil_memory_init();

    Duckvil::Memory::linear_allocator* _mainMemoryAllocator;

    _memoryInterface->m_fnBasicAllocate(&_mainMemoryAllocator, 1024 * 1024 * 32);

#ifdef DUCKVIL_MEMORY_DEBUGGER
    duckvil_memory_debug_info _memoryDebug = {};

    _memoryDebug.m_pAllocator = _mainMemoryAllocator;
    _memoryDebug.m_pParent = nullptr;
    _memoryDebug.m_allocatorType = duckvil_memory_allocator_type_linear;

    _mainMemoryAllocator->m_pDebugData = &_memoryDebug;
    _mainMemoryAllocator->m_fnOnAllocate = Duckvil::Utils::lambda([&](Duckvil::Memory::allocator* _pParentAllocator, Duckvil::Memory::allocator* _pAllocator, duckvil_memory_allocator_type _type)
    {
        duckvil_memory_debug_info* _parentDebugInfo = (duckvil_memory_debug_info*)_pParentAllocator->m_pDebugData;
        duckvil_memory_debug_info* _currentInfo = new duckvil_memory_debug_info{ _pAllocator, _parentDebugInfo, _type };

        _parentDebugInfo->m_aOther.push_back(_currentInfo);

        _pAllocator->m_pDebugData = _currentInfo;
    });
    _mainMemoryAllocator->m_fnOnDeallocate = Duckvil::Utils::lambda([&](Duckvil::Memory::allocator* _pParentAllocator, Duckvil::Memory::allocator* _pAllocator)
    {
        duckvil_memory_debug_info* _parentDebugInfo = (duckvil_memory_debug_info*)_pParentAllocator->m_pDebugData;
        duckvil_memory_debug_info* _currentInfo = _pAllocator->m_pDebugData;

        for(auto it = _parentDebugInfo->m_aOther.begin(); it != _parentDebugInfo->m_aOther.end(); ++it)
        {
            if(*it == _currentInfo)
            {
                _parentDebugInfo->m_aOther.erase(it);

                break;
            }
        }

        delete _currentInfo;
    });
#endif

    Duckvil::Memory::free_list_allocator* _free_list = _memoryInterface->m_fnLinearAllocateFreeListAllocator(_mainMemoryAllocator, 1024 * 1024 * 31);

    Duckvil::__ftable* _engine = duckvil_init();
    Duckvil::__data _engineData;

    if(_parser[Options::SERVER].m_bIsSet)
    {
        _engineData.m_bIsServer = true;

        _engineData.m_usPort = std::stoi(_parser[Options::PORT].m_sResult);
    }
    else if(_parser[Options::CLIENT].m_bIsSet)
    {
        _engineData.m_bIsClient = true;

        _engineData.m_usPort = std::stoi(_parser[Options::PORT].m_sResult);
        _engineData.m_sIP = _parser[Options::IP].m_sResult;
    }

    _engine->init(&_engineData, _memoryInterface, _free_list);

#ifdef DUCKVIL_PLATFORM_WINDOWS
    SetConsoleCtrlHandler(Duckvil::Utils::lambda([&](DWORD _dwCtrlType)
    {
        if(_dwCtrlType == CTRL_CLOSE_EVENT)
        {
            if(!_engine->stop(&_engineData, _engine))
            {
                printf("Failed to close the engine!\n");

                return FALSE;
            }

            return TRUE;
        }

        return FALSE;
    }), true);
#endif

#ifdef DUCKVIL_MEMORY_DEBUGGER
    _engineData.m_pMemoryDebugger = &_memoryDebug;
#endif

    // load project if specified

    if(_parser[Options::PROJECT].m_bIsSet)
    {
        _engineData.m_bufferedEventPool.Broadcast(Duckvil::ProjectManager::LoadProjectEvent{ _parser[Options::PROJECT].m_sResult });
    }

    if(_parser[Options::TRACY].m_bIsSet)
    {
#ifdef DUCKVIL_PLATFORM_WINDOWS
        std::string _command = "Tracy.exe -a ";

        _command += _parser[Options::TRACY].m_sResult;

        startup(&_command[0], DUCKVIL_TRACY_PATH);
#endif
    }

    _engine->start(&_engineData, _engine);

    _memoryInterface->m_fnFreeListClear_(_free_list);
    _memoryInterface->m_fnLinearClear(_mainMemoryAllocator);

    return 0;
}