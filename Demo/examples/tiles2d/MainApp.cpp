#include "MainApp.h"
#include "GameLayer.h"
#include "EditorLayer.h"

MainApp::MainApp()
{
    PushLayer( new GameLayer() );       // In-Game Layer
    PushOverlay( new EditorLayer() );   // Editor UI Layer
}