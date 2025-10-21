#pragma once
#include <string>
#include <imgui.h>

namespace Lumina
{
    class Event;

    class Tab
    {
    public:
        Tab(const std::string& name) : m_Name(name) {}
        virtual ~Tab() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float timestep) {}
        virtual void OnEvent(Event& e) {}
        virtual void OnRender() = 0;

        const std::string& GetName() const { return m_Name; }

        void Show() { m_IsVisible = true; }
        void Hide() { m_IsVisible = false; }
        bool IsVisible() const { return m_IsVisible; }
        void SetVisible(bool visible) { m_IsVisible = visible; }

    protected:
        std::string m_Name;
        bool m_IsVisible = true;
    };
}