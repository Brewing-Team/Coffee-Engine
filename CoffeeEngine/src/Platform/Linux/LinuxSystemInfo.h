#pragma once

#include "CoffeeEngine/Core/SystemInfo.h"

namespace Coffee {

    class LinuxSystemInfo : public SystemInfo
    {
    public:
        uint32_t GetPhysicalProcessorCountImpl() const override;
        uint64_t GetAvailableMemoryImpl() const override;
        uint64_t GetUsedMemoryImpl() const override;
        uint64_t GetProcessMemoryUsageImpl() const override;
    };

}