#pragma once

#include <cstdint>

namespace Coffee {
    /**
     * @brief The SystemInfo class provides information about the Operating System.
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
        static uint32_t GetLogicalProcessorCount();
        static uint32_t GetPhysicalProcessorCount();
        static uint64_t GetTotalMemory();
        static uint64_t GetAvailableMemory();
        static uint64_t GetUsedMemory();
        static uint64_t GetProcessMemoryUsage();

    private:
        static SystemInfo* instance;
    };

}