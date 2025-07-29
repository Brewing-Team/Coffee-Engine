#pragma once

#include <cereal/cereal.hpp>
#include <string>

namespace Coffee
{
    /**
     * @brief Component representing a tag.
     * @ingroup scene
     */
    struct TagComponent
    {
        std::string Tag; ///< The tag string.

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}

        /**
         * @brief Serializes the TagComponent.
         * @tparam Archive The type of the archive.
         * @param archive The archive to serialize to.
         */
        template <class Archive> void serialize(Archive& archive, std::uint32_t const version);
    };
}

CEREAL_CLASS_VERSION(Coffee::TagComponent, 0);
