#pragma once

#include <cereal/cereal.hpp>
#include <glm/glm.hpp>

namespace Coffee {

    enum class AnchorPresetX
    {
        Left,
        Center,
        Right,
        Stretch
    };

    enum class AnchorPresetY
    {
        Top,
        Middle,
        Bottom,
        Stretch
    };

    struct AnchorPreset
    {
        AnchorPresetX X;
        AnchorPresetY Y;

        AnchorPreset() : X(AnchorPresetX::Center), Y(AnchorPresetY::Middle) {}

        AnchorPreset(AnchorPresetX x, AnchorPresetY y) : X(x), Y(y) {}
    };

    struct RectAnchor
    {
        glm::vec2 AnchorMin = { 0.0f, 0.0f };
        glm::vec2 AnchorMax = { 0.0f, 0.0f };

        glm::vec2 OffsetMin = { 0.0f, 0.0f };
        glm::vec2 OffsetMax = { 0.0f, 0.0f };

        void SetAnchorPreset(AnchorPreset preset, const glm::vec4& currentRect, const glm::vec2& parentSize, bool preservePosition);

        glm::vec4 CalculateRect(const glm::vec2& parentSize) const;

        void CalculateTransformData(const glm::vec2& parentSize, glm::vec2& position, glm::vec2& size) const;

        glm::vec4 GetPositionAndSize(const glm::vec2& parentSize) const;

        glm::vec2 GetAnchoredPosition(const glm::vec2& parentSize) const;
        void SetAnchoredPosition(const glm::vec2& position, const glm::vec2& parentSize);
        glm::vec2 GetSize() const;
        void SetSize(const glm::vec2& size, const glm::vec2& parentSize);

        template<class Archive> void save(Archive& archive, std::uint32_t const version) const
        {
            archive(cereal::make_nvp("AnchorMin", AnchorMin),
                    cereal::make_nvp("AnchorMax", AnchorMax),
                    cereal::make_nvp("OffsetMin", OffsetMin),
                    cereal::make_nvp("OffsetMax", OffsetMax));
        }

        template<class Archive> void load(Archive& archive, std::uint32_t const version)
        {
            archive(cereal::make_nvp("AnchorMin", AnchorMin),
                    cereal::make_nvp("AnchorMax", AnchorMax),
                    cereal::make_nvp("OffsetMin", OffsetMin),
                    cereal::make_nvp("OffsetMax", OffsetMax));
        }
    };

} // Coffee