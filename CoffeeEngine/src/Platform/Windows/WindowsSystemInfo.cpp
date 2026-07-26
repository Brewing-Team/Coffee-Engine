#include "WindowsSystemInfo.h"

#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>

namespace Coffee {

    uint32_t WindowsSystemInfo::GetPhysicalProcessorCount() const
    {
        return 0;
    }

    uint64_t WindowsSystemInfo::GetAvailableMemory() const
    {
        return 0;
    }

    uint64_t WindowsSystemInfo::GetUsedMemory() const
    {
        return 0;
    }

    uint64_t WindowsSystemInfo::GetProcessMemoryUsage() const
    {
        PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        return pmc.WorkingSetSize / 1024 / 1024; // Convert from Bytes to MB
    }

}
#endif