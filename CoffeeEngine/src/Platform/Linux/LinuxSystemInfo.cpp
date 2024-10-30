#include "LinuxSystemInfo.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef __linux__

namespace Coffee {

    uint32_t LinuxSystemInfo::GetPhysicalProcessorCountImpl() const
    {
        return 0;
    }

    uint64_t LinuxSystemInfo::GetAvailableMemoryImpl() const
    {
        return 0;
    }

    uint64_t LinuxSystemInfo::GetUsedMemoryImpl() const
    {
        return 0;
    }

    uint64_t LinuxSystemInfo::GetProcessMemoryUsageImpl() const
    {
        FILE *file = fopen("/proc/self/status", "r");
        uint64_t result = 0;
        char line[128];

        if (file == nullptr) {
            return result; // Return 0 if the file cannot be opened
        }

        while (fgets(line, sizeof(line), file) != nullptr)
        {
            if (strncmp(line, "VmRSS:", 6) == 0)
            {
                int i = strlen(line);
                const char *p = line;
                while (*p < '0' || *p > '9')
                    p++;
                line[i - 3] = '\0';
                i = atoi(p);
                result = i / 1024; // Convert from KB to MB
                break;
            }
        }
        fclose(file);
        return result;
    }

}
#endif