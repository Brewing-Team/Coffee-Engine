#pragma once

#include <cereal/cereal.hpp>

namespace Coffee
{
    struct StaticComponent
    {
        StaticComponent() = default;
        StaticComponent(const StaticComponent&) = default;

        template <class Archive> void save(Archive& archive, std::uint32_t const version) const {}
        template <class Archive> void load(Archive& archive, std::uint32_t const version) {}
    };
}

CEREAL_CLASS_VERSION(Coffee::StaticComponent, 0);
