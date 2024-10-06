#pragma once

#include <cereal/cereal.hpp>
#include <filesystem>

namespace cereal {
    template <class Archive>
    void save(Archive& archive, const std::filesystem::path& path)
    {
        archive(path.string());
    }

    template <class Archive>
    void load(Archive& archive, std::filesystem::path& path)
    {
        std::string pathStr;
        archive(pathStr);
        path = std::filesystem::path(pathStr);
    }
}