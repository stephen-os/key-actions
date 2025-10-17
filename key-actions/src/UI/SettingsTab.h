// UI/SettingsTab.h
#pragma once
#include "Tab.h"

namespace Lumina
{
    class SettingsTab : public Tab
    {
    public:
        SettingsTab();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(float timestep) override;
        virtual void OnEvent(Event& e) override;
        virtual void OnRender() override;

    private:
        // Settings state will go here
    };
}