#include "Lumina/Core/Application.h"
#include "Lumina/Core/EntryPoint.h"

#include "NodeGraphTestLayer.h"

Lumina::Application* Lumina::CreateApplication(int argc, char** argv)
{
    Lumina::ApplicationSpecification spec;
    spec.Name = "Nodes Test";
    spec.Width = 900;
    spec.Height = 900;
    
    Lumina::Application* app = new Lumina::Application(spec);
    app->PushLayer<NodeGraphTestLayer>();
    
    return app;
}