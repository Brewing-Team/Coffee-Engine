#pragma once
#include "Resource.h"
#include <cereal/types/polymorphic.hpp>

namespace Coffee {

class ImportSettings {

    public:
        virtual ~ImportSettings() = default;
        virtual ResourceType GetResourceType() const = 0;

        template<typename Archive>
        void serialize(Archive& archive) {}
    };

    class TextureImportSettings : public ImportSettings {
    public:
        bool sRGB = false;
        bool flipY = false;
        bool flipX = false;

        ResourceType GetResourceType() const override { return ResourceType::Texture2D; }

        template<typename Archive>
        void serialize(Archive& archive) {
            archive(sRGB, flipY, flipX);
        }
    };

    class ModelImportSettings : public ImportSettings {
    public:
        float globalScale = 1.0f;
        bool ignoreLight = false;
        bool ignoreCamera = false;

        ResourceType GetResourceType() const override { return ResourceType::Model; }

        template<typename Archive>
        void serialize(Archive& archive) {
            archive(globalScale, ignoreLight, ignoreCamera);
        }
    };

}

CEREAL_REGISTER_TYPE(Coffee::TextureImportSettings)
CEREAL_REGISTER_TYPE(Coffee::ModelImportSettings)