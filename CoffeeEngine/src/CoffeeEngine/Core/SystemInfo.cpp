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
Scope<SystemInfo> SystemInfo::GetSystemInfoInstanceForCurrentPlatform()
{
#ifdef _WIN32
    return CreateScope<WindowsSystemInfo>();
#elif __linux__
    return CreateScope<LinuxSystemInfo>();
#else
#error "Unsupported platform"
#endif
}

uint32_t SystemInfo::GetLogicalProcessorCount() const
    {
        return SDL_GetNumLogicalCPUCores();
    }

    uint64_t SystemInfo::GetTotalMemory() const
    {
        return SDL_GetSystemRAM();
    }
}