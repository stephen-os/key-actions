#pragma once

#include <string>
#include <vector>

#include "Lumina/Utils/UUID.h"
#include "Lumina/Input/GlobalInputPlayback.h"
#include "KeyActions/Core/Memory.h"

#include <imgui_node_editor.h>
#include <glm/vec2.hpp>

namespace KeyActions
{
    using NodeID = ax::NodeEditor::NodeId;
    using PinID = ax::NodeEditor::PinId;
    using LinkID = ax::NodeEditor::LinkId;

    inline constexpr uint64_t NODE_ID_NONE = 0;
    inline constexpr uint64_t PIN_ID_NONE = 0;
    inline constexpr uint64_t LINK_ID_NONE = 0;

    enum class NodeType
    {
        Start = 0,
        End,
        KeyPress,
        KeyRelease,
        MouseMove,
        MousePress,
        MouseRelease,
        MouseScroll,
    };

    enum class PinType
    {
        Undefined = 0,
        Input,
        Output,
    };

    class Node
    {
    public:
        struct Pin
        {
            PinID Id = PIN_ID_NONE;
            LinkID LinkId = LINK_ID_NONE;
            std::string Name = "*Unnamed Pin*";
            PinType Type = PinType::Undefined;
            Ref<Node> ConnectedNode = nullptr;
        };

        Node(const std::string& name);
        virtual ~Node();

        virtual Ref<Node> Execute(Lumina::GlobalInputPlayback* playback) = 0;
        virtual NodeType GetType() const = 0;

        void SetName(const std::string& name);
        const std::string& GetName() const;
        const NodeID& GetNodeID() const;

        std::vector<Pin>& GetPins();
        size_t GetPinCount() const;

        Pin* GetPin(const PinID& id);
        Pin* GetPin(const LinkID& linkId);
        Pin* GetPin(PinType type);
        bool HasPin(PinType type) const;

        static bool CanConnect(PinType sourceType, PinType targetType);
        static bool ConnectPins(Ref<Node> nodeA, PinType pinAType, Ref<Node> nodeB, PinType pinBType);
        static bool DisconnectPin(Ref<Node> node, PinType pinType);

        void SetPosition(const glm::vec2& position);
        glm::vec2 GetPosition() const;

    protected:
        Pin CreatePin(const std::string& name, const PinType type);
        bool AddPin(const Pin& pin);

    protected:
        std::string m_Name = "*Unnamed Node*";
        NodeID m_NodeId = NODE_ID_NONE;
        std::vector<Pin> m_Pins;
        glm::vec2 m_Position = { 0.0f, 0.0f };
    };
}

namespace std
{
    template<>
    struct hash<KeyActions::NodeID>
    {
        size_t operator()(const KeyActions::NodeID& id) const noexcept
        {
            return hash<uint64_t>()(id.Get());
        }
    };

    template<>
    struct hash<KeyActions::PinID>
    {
        size_t operator()(const KeyActions::PinID& id) const noexcept
        {
            return hash<uint64_t>()(id.Get());
        }
    };

    template<>
    struct hash<KeyActions::LinkID>
    {
        size_t operator()(const KeyActions::LinkID& id) const noexcept
        {
            return hash<uint64_t>()(id.Get());
        }
    };
}