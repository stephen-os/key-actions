#include "Lumina/Core/Application.h"
#include "Lumina/Core/EntryPoint.h"

#include "Layers/KeyActionsLayer.h"

Lumina::Application* Lumina::CreateApplication(int argc, char** argv)
{
    Lumina::ApplicationSpecification spec;
    spec.Name = "Key Actions";
    spec.Width = 900;
    spec.Height = 900;
    
    Lumina::Application* app = new Lumina::Application(spec);
	app->PushLayer<KeyActionsLayer>();

    return app;
}