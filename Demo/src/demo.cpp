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
             m_CameraController(m_Camera),
            m_ObjPos(0.0f)
    {
        using namespace Coconuts;    
        
        /**
         * 1) Generate Graphics Context objects:
         *      VA:  Vertex Array
         *      vb:  vertex buffer (with a buffer layout)
         *      ib:  index buffer
         * 
         * 2) Add vb and ib to VA.
         * 
         */
        
        /* Vertex Array (VA) */
        m_VertexArray_square.reset(VertexArray::Create());
        
        float vertices_square[5 * 4] = {
        /* |---- a_Position ----|-- a_TexCoord --| */
            -0.5f, -0.5f,  0.0f,    0.0f, 0.0f,
             0.5f, -0.5f,  0.0f,    1.0f, 0.0f,
             0.5f,  0.5f,  0.0f,    1.0f, 1.0f,
            -0.5f,  0.5f,  0.0f,    0.0f, 1.0f
        };
        
        /* a_Postion: vertex screen position (x,y,z) normalized in the [-1, 1] space */
        
        /* a_TexCoord: texture image edges (x,y) normalized in the [0, 1] space */
        
        /**
         * NOTE:
         * The idea is to map the edges of aa square formed by two intersected triangles
         * (a_Position) to the edges of an image (a_TexCoord).
         * 
         * We then pick the texture's color of each pixel and paint the screen, by
         * interpolating the texture's pixels with corresponding square pixels on 
         * the screen.
         */
     
        
        /* -------------------------------------------------------------------- */
        /* Vertex Buffer (vb) */
        /* -------------------------------------------------------------------- */
        m_VertexBuffer_square.reset(
                    VertexBuffer::Create(vertices_square, sizeof(vertices_square)));
        
        /* (Vertex) Buffer Layout -> Group vertices_square indices in a meaningful way */
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },   /* first 3 floats */
            { ShaderDataType::Float2, "a_TexCoord" }    /* next 2 floats */
        };
        
        /* Associate the layout with a buffer of vertices */
        m_VertexBuffer_square->SetLayout(layout);
        /* -------------------------------------------------------------------- */
        
        
        /* -------------------------------------------------------------------- */
        /* Index Buffer (ib) */
        /* -------------------------------------------------------------------- */
        uint32_t indices_square[6] = {0, 1, 2, 2, 3, 0};
        
        /* Pick indices (lines) of vertices_square that can be re-used to form a square */
        
        /* Add indices to an ib */
        m_IndexBuffer_square.reset(
                IndexBuffer::Create(indices_square, sizeof(indices_square)/sizeof(uint32_t)));
        /* -------------------------------------------------------------------- */
        
        
        /* -------------------------------------------------------------------- */
        /* Add vb and ib to VA */
        m_VertexArray_square->AddVertexBuffer(m_VertexBuffer_square);
        m_VertexArray_square->SetIndexBuffer(m_IndexBuffer_square);
        /* -------------------------------------------------------------------- */
        
        
        /* -------------------------------------------------------------------- */
        /* Create a Shader from files  */
        m_Shader.reset(Shader::Create());
        
        m_Shader->AttachFromFile(
            ShaderTypes::VERTEX, "../assets/shaders/TextureGLSL.vert");
        
        m_Shader->AttachFromFile(
            ShaderTypes::FRAGMENT, "../assets/shaders/TextureGLSL.frag");
        
        m_Shader->DoneAttach(); // FINISH Editing the Shader (Link them)
        /* -------------------------------------------------------------------- */
        
        
        /* Pick an Image and add it a Texture2D object + add filters and interpolations */
        m_Texture2D.reset(Texture2D::Create("../assets/textures/Moris.png"));
        
        m_Coconuts2D.reset(Texture2D::Create("../assets/textures/coconuts.png"));
    }
        
    /**
     * Called once per frame
     */
    void OnUpdate(Coconuts::Timestep ts) override
    {
        //LOG_TRACE("Delta time = {} ms", ts.GetMilliseconds());
        
        using namespace Coconuts;
        Graphics::LowLevelAPI::SetClearColor({0.02f, 0.31f, 0.7f, 1});
        Graphics::LowLevelAPI::Clear();
        
        
        /**
         * Input polling from Keyboard to 
         * move and rotate Camera
         */
#if 0
        /* LEFT */
        if (Polling::IsKeyPressed(Keyboard::KEY_LEFT))
        {
            m_CameraPos.x -= m_CameraMoveSpeed * ts.GetSeconds();
        }
        
        /* RIGHT */
        if (Polling::IsKeyPressed(Keyboard::KEY_RIGHT))
        {
            m_CameraPos.x += m_CameraMoveSpeed * ts.GetSeconds();
        }
        
        /* DOWN - Clockwise rotation */
        if (Polling::IsKeyPressed(Keyboard::KEY_DOWN))
        {
            m_CameraRotation -= m_CameraRotationSpeed * ts.GetSeconds();
        }
        
        /* UP - Counterclockwise rotation */
        if (Polling::IsKeyPressed(Keyboard::KEY_UP))
        {
            m_CameraRotation += m_CameraRotationSpeed * ts.GetSeconds();
        }
#endif
        
        /**
         * Input polling from Keyboard to 
         * move rendered object
         */
        
        /* A - Left */
        if (Polling::IsKeyPressed(Keyboard::KEY_A))
        {
            m_ObjPos.x -= m_ObjMoveSpeed * ts.GetSeconds();
        }
        
        /* D - Right */
        if (Polling::IsKeyPressed(Keyboard::KEY_D))
        {
            m_ObjPos.x += m_ObjMoveSpeed * ts.GetSeconds();
        }
        
        /* W - Up */
        if (Polling::IsKeyPressed(Keyboard::KEY_W))
        {
            m_ObjPos.y += m_ObjMoveSpeed * ts.GetSeconds();
        }
        
        /* S - Down */
        if (Polling::IsKeyPressed(Keyboard::KEY_S))
        {
            m_ObjPos.y -= m_ObjMoveSpeed * ts.GetSeconds();
        }

        
        /* Update Camera's object position */
       // m_Camera.SetPosition(m_CameraPos);
        
        /* Update Camera's object rotation */
        //m_Camera.SetRotation(m_CameraRotation);
        
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_ObjPos);
        
        
        /* Update Camera movement */
        m_CameraController.OnUpdate(ts);
        
        
        /* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
        /* ******************************************************************** */
        /* RENDER CALL:    */
        
        Renderer::BeginScene(m_Camera); // select a camera
        {   
            /**
             * Bind a Texture Object:
             *  Make the shaders use the associated texture image of this Texture
             *  Object in the next Render call.
             */
            m_Texture2D->Bind();
            
            
            /**
             * 1) Select a Shader object to be bound (to be used by GPU) which
             *    contains the compiled source code for a Vertex and a Fragment
             *    shaders.
             * 
             * 2) Select a Vertex Array object which contains the geometry data
             *    to be bound (to be used) by the GPU.
             * 
             * 3) Select a Transform matrix which is hardcoded mapped into a
             *    Vertex Shader Uniform in order to apply a position
             *    transformation to each Vertex Array.
             * 
             *    NOTE: A view projection matrix is auto computed each time a
             *          scene begins as ell (BeginScene)
             * 
             */
            Renderer::Submit(m_Shader, m_VertexArray_square, transform);
            
            
            /**
             * Draw a second texture on top
             */
            m_Coconuts2D->Bind();
            Renderer::Submit(m_Shader, m_VertexArray_square, transform);
        }
        Renderer::EndScene();
        
        /* ******************************************************************** */
        /* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
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
    
    glm::vec3 m_CameraPos = {0.0f, 0.0f, 0.0f};
    float m_CameraMoveSpeed = 1.0f;
    
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 35.0f;    // degrees/seconds
    
    glm::vec3 m_ObjPos;
    float m_ObjMoveSpeed = 1.5f;
    
    /* Graphics Objects */
    
    /* Shader */
    std::shared_ptr<Coconuts::Shader> m_Shader;
    
    /* Textures */
    std::shared_ptr<Coconuts::Texture2D> m_Texture2D, m_Coconuts2D;
    
    /* Buffers */
    std::shared_ptr<Coconuts::VertexArray> m_VertexArray_square;
    std::shared_ptr<Coconuts::VertexBuffer> m_VertexBuffer_square;
    std::shared_ptr<Coconuts::IndexBuffer> m_IndexBuffer_square;
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