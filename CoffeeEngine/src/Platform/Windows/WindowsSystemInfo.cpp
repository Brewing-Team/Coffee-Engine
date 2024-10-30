#include "WindowsSystemInfo.h"

#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>

namespace Coffee {

    uint32_t WindowsSystemInfo::GetPhysicalProcessorCountImpl() const
    {
        return 0;
    }

    uint64_t WindowsSystemInfo::GetAvailableMemoryImpl() const
    {
        return 0;
    }

    uint64_t WindowsSystemInfo::GetUsedMemoryImpl() const
    {
        return 0;
    }

    uint64_t WindowsSystemInfo::GetProcessMemoryUsageImpl() const
    {
        PROCESS_MEMORY_COUNTERS pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        return pmc.WorkingSetSize;
    }

}
#endif