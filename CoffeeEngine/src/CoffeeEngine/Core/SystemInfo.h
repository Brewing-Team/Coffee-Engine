#pragma once

#include <stdint.h>

namespace Coffee {
    /**
     * @defgroup core Core
     * @brief The SystemInfo class provides information about the Operating System.
     * @{
     */
    class SystemInfo
    {
    public:
        virtual ~SystemInfo() = default;

        /**
         * @brief Gets the number of physical processors.
         * @return The number of physical processors.
         */
        virtual uint32_t GetPhysicalProcessorCountImpl() const = 0;

        /**
         * @brief Gets the amount of available memory in the system.
         * @return The amount of available memory in the system.
         */
        virtual uint64_t GetAvailableMemoryImpl() const = 0;

        /**
         * @brief Gets the amount of memory used in the system.
         * @return The amount of memory used in the system.
         */
        virtual uint64_t GetUsedMemoryImpl() const = 0;

        /**
         * @brief Gets the amount of memory used by the process.
         * @return The amount of memory used by the process.
         */
        virtual uint64_t GetProcessMemoryUsageImpl() const = 0;

        // Static methods to delegate to the instance
        static uint32_t GetLogicalProcessorCount(); ///< Gets the number of logical processors.
        static uint32_t GetPhysicalProcessorCount(); ///< Gets the number of physical processors.
        static uint64_t GetTotalMemory(); ///< Gets the total memory in the system.
        static uint64_t GetAvailableMemory(); ///< Gets the available memory in the system.
        static uint64_t GetUsedMemory(); ///< Gets the used memory in the system.
        static uint64_t GetProcessMemoryUsage(); ///< Gets the memory used by the process.

    private:
        static SystemInfo* instance; ///< The instance of the SystemInfo class.
    };

    /** @} */

}