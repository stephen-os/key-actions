#pragma once

#include <string>
#include <vector>

#include "Lumina/Core/Base.h"
#include "Lumina/Utils/UUID.h"
#include "Lumina/Input/GlobalInputPlayback.h"

#include <glm/vec2.hpp>

namespace KeyActions
{
    struct Pin
    {
		uint64_t Id = 0;
		std::string Name;
		bool connected = false;
    };

    class Node
    {
    public:
        using NodePtr = Lumina::Ref<Node>;

        Node(const std::string& name) : m_Name(name), m_NodeID(Lumina::UUID::Generate()) {}
        virtual ~Node() = default;

        virtual NodePtr Execute(Lumina::GlobalInputPlayback* playback) = 0;

        void SetNext(NodePtr next) { m_Next = next; };
        NodePtr GetNext() const { return m_Next; };

        void SetName(const std::string& name) { m_Name = name; };
        const std::string& GetName() const { return m_Name; };

        const uint64_t& GetNodeID() const { return m_NodeID; };

		std::vector<Pin>& GetInputPins() { return m_InputPins; }
		std::vector<Pin>& GetOutputPins() { return m_OutputPins; }

        void SetPosition(const glm::vec2& position) { m_Position = position; };
        glm::vec2 GetPosition() const { return m_Position; };

    protected:
        Pin CreatePin(std::string_view name)
        {
            Pin pin;
            pin.Id = Lumina::UUID::Generate();
			pin.Name = name;
            return pin;
		}

    protected:
        std::string m_Name;

        uint64_t m_NodeID;
		std::vector<Pin> m_InputPins;
		std::vector<Pin> m_OutputPins;

        NodePtr m_Next = nullptr;

        glm::vec2 m_Position = { 0.0f, 0.0f };
    };
}