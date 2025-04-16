#include "UIAnchor.h"

namespace Coffee {

    void RectAnchor::SetAnchorPreset(AnchorPreset preset, const glm::vec4& currentRect, const glm::vec2& parentSize, bool preservePosition)
    {
        glm::vec2 currentSize = { currentRect.z, currentRect.w };

        switch (preset.X) {
            case AnchorPresetX::Left:
                AnchorMin.x = 0.0f;
                AnchorMax.x = 0.0f;
                break;
            case AnchorPresetX::Center:
                AnchorMin.x = 0.5f;
                AnchorMax.x = 0.5f;
                break;
            case AnchorPresetX::Right:
                AnchorMin.x = 1.0f;
                AnchorMax.x = 1.0f;
                break;
            case AnchorPresetX::Stretch:
                AnchorMin.x = 0.0f;
                AnchorMax.x = 1.0f;
                break;
        }

        switch (preset.Y) {
            case AnchorPresetY::Top:
                AnchorMin.y = 0.0f;
                AnchorMax.y = 0.0f;
                break;
            case AnchorPresetY::Middle:
                AnchorMin.y = 0.5f;
                AnchorMax.y = 0.5f;
                break;
            case AnchorPresetY::Bottom:
                AnchorMin.y = 1.0f;
                AnchorMax.y = 1.0f;
                break;
            case AnchorPresetY::Stretch:
                AnchorMin.y = 0.0f;
                AnchorMax.y = 1.0f;
                break;
        }

        if (preservePosition)
        {
            OffsetMin.x = currentRect.x - parentSize.x * AnchorMin.x;
            OffsetMin.y = currentRect.y - parentSize.y * AnchorMin.y;
            OffsetMax.x = OffsetMin.x + currentSize.x;
            OffsetMax.y = OffsetMin.y + currentSize.y;
        }
        else
        {
            if (currentSize.x == 0 && currentSize.y == 0)
            {
                currentSize = { 100, 100 };
            }

            if (AnchorMin.x == AnchorMax.x)
            {
                float pivotOffsetX = currentSize.x * Pivot.x;
                OffsetMin.x = -pivotOffsetX;
                OffsetMax.x = currentSize.x - pivotOffsetX;
            }
            else
            {
                OffsetMin.x = 0;
                OffsetMax.x = 0;
            }

            if (AnchorMin.y == AnchorMax.y)
            {
                float pivotOffsetY = currentSize.y * Pivot.y;
                OffsetMin.y = -pivotOffsetY;
                OffsetMax.y = currentSize.y - pivotOffsetY;
            }
            else
            {
                OffsetMin.y = 0;
                OffsetMax.y = 0;
            }
        }
    }

    glm::vec4 RectAnchor::CalculateRect(const glm::vec2& parentSize) const
    {
        float left = parentSize.x * AnchorMin.x + OffsetMin.x;
        float top = parentSize.y * AnchorMin.y + OffsetMin.y;
        float right = parentSize.x * AnchorMax.x + OffsetMax.x;
        float bottom = parentSize.y * AnchorMax.y + OffsetMax.y;

        return { left, top, right - left, bottom - top };
    }

    void RectAnchor::CalculateTransformData(const glm::vec2& parentSize, glm::vec2& position, glm::vec2& size) const
    {
        glm::vec4 rect = CalculateRect(parentSize);
        float width = rect.z;
        float height = rect.w;

        float pivotOffsetX = width * Pivot.x;
        float pivotOffsetY = height * Pivot.y;

        position.x = rect.x + pivotOffsetX;
        position.y = rect.y + pivotOffsetY;

        size = { width, height };
    }

    glm::vec4 RectAnchor::GetPositionAndSize(const glm::vec2& parentSize) const
    {
        glm::vec4 rect = CalculateRect(parentSize);
        return { rect.x, rect.y, rect.z, rect.w };
    }

    glm::vec2 RectAnchor::GetAnchoredPosition(const glm::vec2& parentSize) const
    {
        float absoluteX = parentSize.x * AnchorMin.x + OffsetMin.x;
        float absoluteY = parentSize.y * AnchorMin.y + OffsetMin.y;
        float width = OffsetMax.x - OffsetMin.x;
        float height = OffsetMax.y - OffsetMin.y;
        return {
            absoluteX + width * Pivot.x,
            absoluteY + height * Pivot.y
        };
    }

    void RectAnchor::SetAnchoredPosition(const glm::vec2& position, const glm::vec2& parentSize)
    {
        if (AnchorMin.x == AnchorMax.x && AnchorMin.y == AnchorMax.y)
        {
            glm::vec2 currentPivotPos = GetAnchoredPosition(parentSize);
            glm::vec2 delta = position - currentPivotPos;

            OffsetMin.x += delta.x;
            OffsetMin.y += delta.y;
            OffsetMax.x += delta.x;
            OffsetMax.y += delta.y;
        }
    }

    glm::vec2 RectAnchor::GetSize() const
    {
        return { OffsetMax.x - OffsetMin.x, OffsetMax.y - OffsetMin.y };
    }

    void RectAnchor::SetSize(const glm::vec2& size, const glm::vec2& parentSize)
    {
        if (AnchorMin.x == AnchorMax.x && AnchorMin.y == AnchorMax.y)
        {
            glm::vec2 currentPos = GetAnchoredPosition(parentSize);

            OffsetMin.x = currentPos.x - size.x * Pivot.x;
            OffsetMin.y = currentPos.y - size.y * Pivot.y;
            OffsetMax.x = OffsetMin.x + size.x;
            OffsetMax.y = OffsetMin.y + size.y;
        }
    }

    void RectAnchor::SetPivot(const glm::vec2& newPivot, const glm::vec2& parentSize)
    {
        if (AnchorMin.x == AnchorMax.x && AnchorMin.y == AnchorMax.y)
        {
            glm::vec2 worldPosition = GetAnchoredPosition(parentSize);
            Pivot = newPivot;
            SetAnchoredPosition(worldPosition, parentSize);
        }
        else
        {
            Pivot = newPivot;
        }
    }
} // Coffee