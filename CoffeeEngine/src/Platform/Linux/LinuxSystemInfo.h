#pragma once

#include "CoffeeEngine/Core/SystemInfo.h"

namespace Coffee {

    class LinuxSystemInfo : public SystemInfo
    {
    public:
        uint32_t GetPhysicalProcessorCount() const override;
        uint64_t GetAvailableMemory() const override;
        uint64_t GetUsedMemory() const override;
        uint64_t GetProcessMemoryUsage() const override;
    };

}