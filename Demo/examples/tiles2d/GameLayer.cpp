#include <string>
#include <coconuts/Renderer.h>
#include "GameLayer.h"

#define GAMELAYER_SPRITESHEET_PATH  "../assets/textures/RPGpack_sheet_2X.png";

std::shared_ptr<Coconuts::Framebuffer> GameLayer::s_Framebuffer;

void GameLayer::OnUpdate(Coconuts::Timestep ts)
{
    /* Update Camera Controller */
    m_CameraController.OnUpdate(ts);
    
    Coconuts::Graphics::LowLevelAPI::SetClearColor({0.0f, 0.0f, 0.0f, 1});
    Coconuts::Graphics::LowLevelAPI::Clear();
    
    /* Bind Framebuffer */
    s_Framebuffer->Bind();
    
    /* Set backgound color*/
    Coconuts::Graphics::LowLevelAPI::SetClearColor({0.02f, 0.31f, 0.7f, 1});
    Coconuts::Graphics::LowLevelAPI::Clear();
    
    /* Render */
    Coconuts::Renderer2D::ResetStatistics();
    Coconuts::Renderer2D::BeginScene(m_Camera);
    
    Coconuts::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 2.0f}, m_TreeSprite);
    
    Coconuts::Renderer2D::EndScene();
    
    /* Unbind Framebuffer */
    s_Framebuffer->Unbind();
}

void GameLayer::OnEvent(Coconuts::Event& event)
{
    /* Update CameraController */
    m_CameraController.OnEvent(event);
}

GameLayer::GameLayer()
    : m_CameraAR_x(16.0f),
      m_CameraAR_y(9.0f),
      m_ZoomLevel(5.0f),
      m_Camera(-m_CameraAR_x * m_ZoomLevel, -m_CameraAR_x * m_ZoomLevel, -m_CameraAR_y * m_ZoomLevel, m_CameraAR_y * m_ZoomLevel),
      m_CameraController(m_Camera, m_CameraAR_x, m_CameraAR_y, m_ZoomLevel)
{
    float aspectRatio = (float) (m_CameraAR_x / m_CameraAR_y);
    m_Camera.SetProjection(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
}

void GameLayer::OnAttach()
{
    /* Load Spritesheet */
    const std::string path = GAMELAYER_SPRITESHEET_PATH;
    m_SpritesheetTexture2D.reset( Coconuts::Texture2D::Create(path) );
    
    /* Load Sprites */
    m_TreeSprite.reset( Coconuts::Sprite::Create(m_SpritesheetTexture2D, {2, 1}, {128, 128}, {1, 2}) );
    
    /* Create Framebuffer */
    Coconuts::FramebufferSpecification spec;
    spec.width = 1280;
    spec.height = 696;
    
    s_Framebuffer.reset( Coconuts::Framebuffer::Create(spec) );
}

void GameLayer::OnDetach()
{
    
}