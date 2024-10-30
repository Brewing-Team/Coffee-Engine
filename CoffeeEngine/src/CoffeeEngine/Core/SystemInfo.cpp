#include "SystemInfo.h"
#include "SDL3/SDL_cpuinfo.h"
#include <sys/types.h>

#ifdef _WIN32
#include "Platform/Windows/WindowsSystemInfo.h"
#elif __linux__
#include "Platform/Linux/LinuxSystemInfo.h"
#endif

namespace Coffee
{
    SystemInfo* SystemInfo::instance = []() -> SystemInfo* {
        #ifdef _WIN32
            return new WindowsSystemInfo();
        #elif __linux__
            return new LinuxSystemInfo();
        /* #elif __APPLE__
            return new SystemInfoMac(); */
        #else
            #error "Unsupported platform"
        #endif
    }();

    uint32_t SystemInfo::GetLogicalProcessorCount()
    {
        return SDL_GetNumLogicalCPUCores();
    }

    uint64_t SystemInfo::GetTotalMemory()
    {
        return SDL_GetSystemRAM();
    }

    uint32_t SystemInfo::GetPhysicalProcessorCount()
    {
        return instance->GetPhysicalProcessorCountImpl();
    }

    uint64_t SystemInfo::GetAvailableMemory()
    {
        return instance->GetAvailableMemoryImpl();
    }

    uint64_t SystemInfo::GetUsedMemory()
    {
        return instance->GetUsedMemoryImpl();
    }

    uint64_t SystemInfo::GetProcessMemoryUsage()
    {
        return instance->GetProcessMemoryUsageImpl();
    }
}