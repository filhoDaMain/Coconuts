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
#include "demo.h"
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
            m_CameraController(m_Camera)
    {
        /* Init Texture image */
        m_MorisTexture.reset(Coconuts::Texture2D::Create("../assets/textures/Moris.png"));
        m_CheckerboardTexture.reset(Coconuts::Texture2D::Create("../assets/textures/checkerboard.png"));
        m_CoconutsTextTexture.reset(Coconuts::Texture2D::Create("../assets/textures/Coconuts.png"));
    }
        
    /**
     * Called once per frame
     */
    void OnUpdate(Coconuts::Timestep ts) override
    {
        m_CameraController.OnUpdate(ts);
        
        Coconuts::Graphics::LowLevelAPI::SetClearColor({0.02f, 0.31f, 0.7f, 1});
        Coconuts::Graphics::LowLevelAPI::Clear();
        
        Coconuts::Renderer2D::BeginScene(m_Camera);
        Coconuts::Renderer2D::DrawRotatedQuad({0.0f, 0.0f}, {2.0f, 2.0f}, 0.8f, m_CheckerboardTexture, 10.0f, {0.5f, 0.7f, 0.8f, 1.0f});
        Coconuts::Renderer2D::DrawQuad({-0.7f, 0.0f}, {0.8f, 0.8f}, {0.8f, 1.0f, 0.1f, 1.0f});
        Coconuts::Renderer2D::DrawRotatedQuad({0.5f, -0.3f}, {0.5f, 0.75f}, 0.4f, {0.5f, 0.1f, 0.5f, 1.0f});
        Coconuts::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, m_MorisTexture);
        Coconuts::Renderer2D::DrawQuad({0.0f, 0.7f}, {1.0f, 1.0f}, m_CoconutsTextTexture);
        Coconuts::Renderer2D::EndScene();
    }
        
    /**
     * Triggered whenever an Event occurs
     */
    void OnEvent(Coconuts::Event& event) override
    {
        /* Log the event */
        //LOG_TRACE(event.ToString());
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
};



/* GUI - Settings */
/* ------------------------------------------------------------ */
class GUI_ColorSettings : public ::Coconuts::Editor::GUILayer
{
public:
    GUI_ColorSettings() 
    {
        
    }
    
    ~GUI_ColorSettings()
    {
        
    }
    
    void OnUpdate(Coconuts::Timestep ts) override
    {   
        
    }
    
private:
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