#pragma once

namespace Coffee {

    class Panel
    {
    public:
        virtual ~Panel() = default;
        virtual void OnImGuiRender() = 0;
        bool IsVisible() const { return m_Visible; }
        void ToggleVisibility() { m_Visible = !m_Visible; }
    protected:
        bool m_Visible = true;
    };

}