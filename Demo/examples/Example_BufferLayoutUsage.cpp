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
         */
        
        /* Vertex Array (VA) */
        m_VertexArray.reset(VertexArray::Create());
        
        /* Vertices */
        float vertices[3 * 7] = {
        /*  | x      y     z |       RGBA Color       |*/
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
        /*  |----------------- STRIDE ----------------|*/
        };
     
        
        /* -------------------------------------------------------------------- */
        /* Vertex Buffer (vb) */
        /* -------------------------------------------------------------------- */
        m_VertexBuffer.reset(
                VertexBuffer::Create(vertices, sizeof(vertices))); // Bound
        
            /* (Vertex) Buffer Layout */
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
            };
            m_VertexBuffer->SetLayout(layout);
        /* -------------------------------------------------------------------- */
        
        
        /* -------------------------------------------------------------------- */
        /* Index Buffer (ib) */
        /* -------------------------------------------------------------------- */
        uint32_t indices[3] = {0, 1, 2};
        
        /* Add indices to an ib */
        m_IndexBuffer.reset(
                IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
        /* -------------------------------------------------------------------- */
        
        
        /* -------------------------------------------------------------------- */
        /* Add vb and ib to VA */
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        /* -------------------------------------------------------------------- */
        
        
        /* Wrinting Shaders */
        std::string vertexSrc = R"(
            
                #version 330 core
                
                layout(location = 0) in vec3 a_Position;
                layout(location = 1) in vec4 a_Color;
                
                uniform mat4 u_ViewProj;
                uniform mat4 u_Transform;
                
                out vec3 v_Position;
                out vec4 v_Color;
                
                void main()
                {
                    v_Position = a_Position;
                    v_Color = a_Color;
                    gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
                }
                
            )";
        
        std::string fragmentSrc = R"(
            
                #version 330 core
                
                layout(location = 0) out vec4 color;
                
                in vec3 v_Position;
                in vec4 v_Color;
                
                void main()
                {
                    color = vec4(v_Position * 0.5 + 0.5, 1.0);
                
                    color = v_Color;
                }
                
            )";
        
        //m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
        m_Shader.reset(Shader::Create(vertexSrc, fragmentSrc));
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
        m_Camera.SetPosition(m_CameraPos);
        
        /* Update Camera's object rotation */
        m_Camera.SetRotation(m_CameraRotation);
        
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_ObjPos);
        
        /* ******************************************************************** */
        /* RENDER: :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
        Renderer::BeginScene(m_Camera); // select a camera
        {
            Renderer::Submit(m_Shader, m_VertexArray, transform);
        }
        Renderer::EndScene();
        /* ******************************************************************** */
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
    
    glm::vec3 m_CameraPos = {0.0f, 0.0f, 0.0f};
    float m_CameraMoveSpeed = 1.0f;
    
    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 35.0f;    // degrees/seconds
    
    glm::vec3 m_ObjPos;
    float m_ObjMoveSpeed = 1.5f;
    
    /* Graphics Objects */
    std::shared_ptr<Coconuts::Shader> m_Shader;
    std::shared_ptr<Coconuts::VertexArray> m_VertexArray;
    std::shared_ptr<Coconuts::VertexBuffer> m_VertexBuffer;
    std::shared_ptr<Coconuts::IndexBuffer> m_IndexBuffer;
};


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
        
#if 0
        /**
         * Editor GUI.
         */
        using namespace Coconuts;
        Editor::GUILayer* gui = new Editor::GUILayer();
        this->PushOverlay(gui);
#endif
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
