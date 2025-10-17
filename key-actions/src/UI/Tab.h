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

    protected:
        std::string m_Name;
    };
}