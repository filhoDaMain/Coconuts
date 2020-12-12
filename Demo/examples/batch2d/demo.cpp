/**
 * A Sandbox Application example to show Coconuts running.
 */

//!!!
/* Must define __COCONUTS_SANDBOX_APP__ prior to any Coconuts include */
#ifndef __COCONUTS_SANDBOX_APP__
#define __COCONUTS_SANDBOX_APP__
#endif  // __COCONUTS_SANDBOX_APP__
//!!!

/* Core Library stuff */
#include <coconuts/core.h>
#include <coconuts/Logger.h>
#include <coconuts/Application.h>
#include <coconuts/Layer.h>
#include <coconuts/EventSystem.h>
#include <coconuts/Polling.h>
#include <coconuts/Keyboard.h>
#include <coconuts/Mouse.h>
#include <coconuts/Renderer.h>

/* Editor Library stuff */
#include <coconuts/editor.h>

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CameraController.h"
#include <fstream>
#include <streambuf>

/**
 * Define a new custom Layer, inherited from Coconuts::Layer base class.
 * 
 * Event triggers and draw calls are issued per Layer.
 * Overlays get always on top of the last inserted Layer.
 * 
 * The rendering order of Layers/Overlays is as follows:
 * 
 *      Further away
 *  +-----------+
 *  |   Layer   |   0
 *  |   +-----------+
 *  +---|   Layer   |   1
 *      |   +-----------+
 *      +---|   Layer   |   2
 *          |   +-----------+ <----------- Next Layer will be pushed here
 *          +---|  Overlay  |   3
 *              |   +-----------+
 *              +---|  Overlay  |
 *                  |           |   4
 *                  +-----------+ <------- Next Overlay will be pushed here
 *      TOP VIEW
 */
class ExampleLayer : public ::Coconuts::Layer
{
public:
    ExampleLayer(const std::string& layerName)
        :   Layer(layerName),
            m_Camera(-1.6f, 1.6f, -0.9f, 0.9f),
            m_CameraController(m_Camera),
            m_MorisPosX(0.0f),
            m_MorisPosY(0.0f)
    {
        /* Init Texture image */
        m_MorisTexture.reset(Coconuts::Texture2D::Create("../assets/textures/Moris.png"));
        m_CheckerboardTexture.reset(Coconuts::Texture2D::Create("../assets/textures/Checkerboard.png"));
        m_CoconutsTextTexture.reset(Coconuts::Texture2D::Create("../assets/textures/Coconuts.png"));
        
        /* Init Spritesheet texture image */
        m_AnimalsSpritesheet.reset(Coconuts::Texture2D::Create("../assets/textures/square_nodetailsOutline.png"));
        
        /* Init Sprites */
        m_SpriteCow.reset(Coconuts::Sprite::Create(m_AnimalsSpritesheet, {544, 136}, {136, 136}));
        m_SpriteMonkey.reset(Coconuts::Sprite::Create(m_AnimalsSpritesheet, {544, 680}, {136, 136}));
        m_SpritePenguin.reset(Coconuts::Sprite::Create(m_AnimalsSpritesheet, {136, 272}, {136, 136}));
        m_SpritePig.reset(Coconuts::Sprite::Create(m_AnimalsSpritesheet, {136, 136}, {136, 136}));
    }
        
    /**
     * Called once per frame
     */
    void OnUpdate(Coconuts::Timestep ts) override
    {   
        MyInputPolling(ts);
        
        m_CameraController.OnUpdate(ts);
        
        Coconuts::Graphics::LowLevelAPI::SetClearColor({0.02f, 0.31f, 0.7f, 1});
        Coconuts::Graphics::LowLevelAPI::Clear();
        
        // Debug renderer statistics
        Coconuts::Renderer2D::ResetStatistics();
        {
            Coconuts::Renderer2D::BeginScene(m_Camera);
            
            // Checkerboard texture (same texture slot)
            Coconuts::Renderer2D::DrawQuad({0.0f, 0.0f}, {2.0f, 2.0f}, m_CheckerboardTexture, s_CheckerboardTilingFactor, s_CheckerBoardTint);
            Coconuts::Renderer2D::DrawQuad({-0.7f, -0.7f}, {1.0f, 1.0f}, m_CheckerboardTexture, 1.0f);
            
            // Flat Colors (same texture slot)
            Coconuts::Renderer2D::DrawQuad({-0.9f, 1.0f}, {0.8f, 0.8f}, {0.8f, 1.0f, 0.1f, 1.0f});
            Coconuts::Renderer2D::DrawQuad({-0.7f, 0.0f}, {0.8f, 0.8f}, {0.8f, 1.0f, 0.1f, 1.0f});
            Coconuts::Renderer2D::DrawRotatedQuad({0.5f, -0.3f}, {0.5f, 0.75f}, s_MorisRotation, {0.5f, 0.1f, 0.5f, 1.0f});
            
            // Moris texture (same texture slot)
            Coconuts::Renderer2D::DrawRotatedQuad({m_MorisPosX, m_MorisPosY}, {s_MorisScale, s_MorisScale}, s_MorisRotation,  m_MorisTexture);
            Coconuts::Renderer2D::DrawRotatedQuad({-0.5f, -0.5f}, {0.2f, 0.2f}, 0.0f,  m_MorisTexture);
            
            /* Draw Sprites from same Spritesheet (same texture slot) */
            Coconuts::Renderer2D::DrawQuad({-0.5f,  0.5f}, {0.5f, 0.5f}, m_SpriteCow);
            Coconuts::Renderer2D::DrawQuad({ 0.5f,  0.5f}, {0.5f, 0.5f}, m_SpriteMonkey);
            Coconuts::Renderer2D::DrawQuad({-0.5f, -0.5f}, {0.5f, 0.5f}, m_SpritePenguin);
            Coconuts::Renderer2D::DrawQuad({ 0.5f, -0.5f}, {0.5f, 0.5f}, m_SpritePig);
            
            // Coconuts logo texture (same texture slot)
            Coconuts::Renderer2D::DrawQuad({0.0f, 0.7f}, {1.0f, 1.0f}, m_CoconutsTextTexture);
            
            Coconuts::Renderer2D::EndScene();
        }
    }
       
    void MyInputPolling(Coconuts::Timestep& ts)
    {
        /* Input Polling -> Move Moris */
        // VERTICAL
        if (Coconuts::Polling::IsKeyPressed(Coconuts::Keyboard::KEY_W))
        {
            m_MorisPosY += 0.7f * ts.GetSeconds();
        }
        if (Coconuts::Polling::IsKeyPressed(Coconuts::Keyboard::KEY_S))
        {
            m_MorisPosY -= 0.7f * ts.GetSeconds();
        }
        
        // HORIZONTAL
        if (Coconuts::Polling::IsKeyPressed(Coconuts::Keyboard::KEY_D))
        {
            m_MorisPosX += 0.7f * ts.GetSeconds();
        }
        if (Coconuts::Polling::IsKeyPressed(Coconuts::Keyboard::KEY_A))
        {
            m_MorisPosX -= 0.7f * ts.GetSeconds();
        }
    }
    
    /**
     * Triggered whenever an Event occurs
     */
    void OnEvent(Coconuts::Event& event) override
    {

    }
    
    static void SetCheckerBoardTint(const glm::vec3& tint)
    {
        s_CheckerBoardTint = {tint.x, tint.y, tint.z, 1.0f};
    }
    
    static void SetCheckerboardTilingFactor(float factor)
    {
        s_CheckerboardTilingFactor = factor;
    }
    
    static void SetMorisScale(float scale)
    {
        s_MorisScale = scale;
    }
    
    static void SetMorisRotation(float rotation)
    {
        s_MorisRotation = rotation;
    }
    
private:
    /* Camera */
    Coconuts::OrthographicCamera m_Camera;
    
    /* Camera Controller */
    CameraController m_CameraController;
    
    /* Textures */
    std::shared_ptr<Coconuts::Texture2D> m_MorisTexture;
    std::shared_ptr<Coconuts::Texture2D> m_CheckerboardTexture;
    std::shared_ptr<Coconuts::Texture2D> m_CoconutsTextTexture;
    
    /* Sprite Sheet */
    std::shared_ptr<Coconuts::Texture2D> m_AnimalsSpritesheet;
    
    /* Sprites */
    std::shared_ptr<Coconuts::Sprite> m_SpriteCow;
    std::shared_ptr<Coconuts::Sprite> m_SpriteMonkey;
    std::shared_ptr<Coconuts::Sprite> m_SpritePenguin;
    std::shared_ptr<Coconuts::Sprite> m_SpritePig;
    
    static glm::vec4 s_CheckerBoardTint;
    static float s_CheckerboardTilingFactor;
    static float s_MorisScale;
    static float s_MorisRotation;
    float m_MorisPosX;
    float m_MorisPosY;
};

glm::vec4 ExampleLayer::s_CheckerBoardTint = glm::vec4(1.0f);
float ExampleLayer::s_CheckerboardTilingFactor = 1.0f;
float ExampleLayer::s_MorisScale = 1.0f;
float ExampleLayer::s_MorisRotation = 0.0f;


/* GUI - Settings */
/* ------------------------------------------------------------ */
class GUI_ColorSettings : public ::Coconuts::Editor::GUILayer
{
public:
    GUI_ColorSettings()
        : m_CheckerBoardTint(glm::vec3(1.0f)), m_TilingFactor(1.0f), m_Scale(1.0f), m_Rotation(0.0f)
    {
        
    }
    
    ~GUI_ColorSettings()
    {
        
    }
    
    void OnUpdate(Coconuts::Timestep ts) override
    {   
        /* Get Live statistics */
        stats = Coconuts::Renderer2D::GetStatistics();
        
#if 0
        /* New ImGui Window */
        //------------------------------------------------------------------------
        ImGui::Begin("Checkerboard Settings");
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
        
        ImGui::ColorEdit3("Tint", glm::value_ptr(m_CheckerBoardTint));
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
        
        ImGui::SliderFloat("Tiling", &m_TilingFactor, 1.0f, 10.f);
        ImGui::End();
        //------------------------------------------------------------------------
        
        
        /* New ImGui Window */
        //------------------------------------------------------------------------
        ImGui::Begin("Moris Settings");
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
        ImGui::SliderFloat("Scale", &m_Scale, 1.0f, 10.f);
        
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
        
        ImGui::SliderAngle("Rotation", &m_Rotation, -180.f, 180.f);
        ImGui::End();
        //------------------------------------------------------------------------
#endif
        
        /* New ImGui Window */
        //------------------------------------------------------------------------
        ImGui::Begin("Statistics");
        ImGui::Text("Batch render statistics:");
        ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
        
        ImGui::Text("%d Draw Calls", stats.drawCalls);
        ImGui::Spacing();
        ImGui::Text("%d Quads", stats.quadCount);
        
        ImGui::End();
        //------------------------------------------------------------------------
        
        
        /* Pass picked color */
        //ExampleLayer::SetCheckerBoardTint(m_CheckerBoardTint);
        
        /* Pass selected tiling factor */
        //ExampleLayer::SetCheckerboardTilingFactor(m_TilingFactor);
        
        /* Pass selected scale */
        //ExampleLayer::SetMorisScale(m_Scale);
        
        /* Pass selected rotation */
        //ExampleLayer::SetMorisRotation(m_Rotation);
    }
    
private:
    Coconuts::Renderer2DStatistics stats;   /* Renderer2D live statistics */
    
    glm::vec3 m_CheckerBoardTint;
    float m_TilingFactor;
    float m_Scale;
    float m_Rotation;
};
/* ------------------------------------------------------------ */


/**
 * Define a new Application, inherited from Coconuts::Application base clase
 */
class DemoApp : public ::Coconuts::Application
{
public:
    DemoApp()
    {
        /**
         * Create a new Layer and push it to the LayerStack.
         */
        ExampleLayer* newLayer = new ExampleLayer("Layer0");
        this->PushLayer(newLayer);
       
        /**
         * GUI
         */
        GUI_ColorSettings* gui = new GUI_ColorSettings();
        this->PushOverlay(gui);
    }
    
    ~DemoApp()
    {
        
    }
};


/**
 * Feed Coconuts with a new user defined Application that
 * inherits from Coconuts::Application base class
 */
Coconuts::Application* Coconuts::CreateApplication()
{
    return new DemoApp();
}