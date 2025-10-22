#pragma once

#include "Lumina/Core/Layer.h"

#include "KeyActions/UI/Tab.h"

#include <memory>
#include <vector>

namespace KeyActions
{
    class KeyActionsLayer : public Lumina::Layer
    {
    public:
        KeyActionsLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;
        virtual void OnUpdate(float timestep) override;
        virtual void OnUIRender() override;

    private:
        std::vector<std::shared_ptr<Tab>> m_Tabs;

		size_t m_ActiveTabIndex = 0;
    };
} 