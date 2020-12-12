#include <coconuts/Renderer.h>
#include "GameLayer.h"


void GameLayer::OnUpdate(Coconuts::Timestep ts)
{
    /* Set backgound color*/
    Coconuts::Graphics::LowLevelAPI::SetClearColor({0.02f, 0.31f, 0.7f, 1});
    Coconuts::Graphics::LowLevelAPI::Clear();
    
    /* Render */
    Coconuts::Renderer2D::BeginScene(m_Camera);
    
    Coconuts::Renderer2D::EndScene();
}



GameLayer::GameLayer()
    : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
{
    
}

void GameLayer::OnAttach()
{
    
}

void GameLayer::OnDetach()
{
    
}

void GameLayer::OnEvent(Coconuts::Event& event)
{
    
}