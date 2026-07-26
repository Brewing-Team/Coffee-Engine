#pragma once

#include "Base.h"
#include <cstdint>

namespace Coffee {
    /**
     * @defgroup core Core
     * @brief The ISystemInfo class provides information about the Operating System.
     * @{
     */
    class SystemInfo
    {
    public:
        virtual ~SystemInfo() = default;

        static Scope<SystemInfo> GetSystemInfoInstanceForCurrentPlatform();

        virtual uint32_t GetLogicalProcessorCount() const; ///< Gets the number of logical processors.
        virtual uint32_t GetPhysicalProcessorCount() const; ///< Gets the number of physical processors.
        virtual uint64_t GetTotalMemory() const; ///< Gets the total memory in the system.
        virtual uint64_t GetAvailableMemory() const; ///< Gets the available memory in the system.
        virtual uint64_t GetUsedMemory() const; ///< Gets the used memory in the system.
        virtual uint64_t GetProcessMemoryUsage() const; ///< Gets the memory used by the process.
    };

    /** @} */

}