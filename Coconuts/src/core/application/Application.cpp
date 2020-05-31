/*
 * Copyright 2020 Andre Temprilho
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <coconuts/Application.h>
#include <coconuts/core.h>
#include <coconuts/EventSystem.h>
#include <coconuts/Logger.h>
#include <functional>
#include <glad/glad.h>
#include <string>
#include <stdint.h>


namespace Coconuts
{
    /* Singleton Pattern */
    Application* Application::s_Instance = nullptr;
    
    Application::Application()
    {
        /* Assert that the Singleton Pattern is respected */
        if (s_Instance != nullptr)
        {
            LOG_CRITICAL("Crash! Application already exists.");
            exit(1);
        }
        
        s_Instance = this;
        
        LOG_DEBUG("Sandbox App created!");
        p_Window = std::unique_ptr<Window>(Window::Create());
        
#if defined(__APPLE__)
        if (p_Window != nullptr)
        {
            LOG_DEBUG("Coconuts WindowSystem initialized for the MacOS Platform");
        }
        //ELSE - it should already have crashed!
#elif __gnu_linux__
        if (p_Window != nullptr)
        {
            LOG_DEBUG("Coconuts WindowSystem initialized for the GNU Platform");
        }
        //ELSE - it should already have crashed!
#endif
        
        /* Initialize the Window Manager library callbacks */
        if ( p_Window->InitWindowManagerCallbacks("GLFW") )
        {
            LOG_DEBUG("Window Manager library callbacks successfully initialized");
        }
        else
        {
            LOG_ERROR("Failed to initialize Window Manager library callbacks!");
        }
        
        /* Set the callback function for all Window Manager library events */
        p_Window->SetEventCallback( std::bind(&Application::OnEvent, this, std::placeholders::_1) );
        
        /**
         * Generate Graphics Context objects
         */
        /* Vertex Array (VA) */
        glGenVertexArrays(1, &m_VA);
        glBindVertexArray(m_VA);                /* bind VA */
        
        /* Position Vertices (normalized in the [-1, 1] space) */
        float vertices[3 * 3] = {       /* 3 vertices of 3D positions */
        /*    x      y     z    */
            -0.5f, -0.5f, 0.0f, // Index 0
             0.5f, -0.5f, 0.0f, // Index 1
             0.0f,  0.5f, 0.0f  // Index 2
        /*  |---- STRIDE ----|*/
        };
     
        /* Vertex Buffer (VB) */
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices))); // Bound
        
        /* Enable a vertex attribute */
        glEnableVertexAttribArray(0 /* Enable index 0 */);
        
        /* Vertex Attribute 0 - Buffer Layout (how data is structured) */
        glVertexAttribPointer(0                 /* index nr */,
                              3                 /* number of elements per vertex */,
                              GL_FLOAT          /* type of vertex elements */,
                              GL_FALSE          /* non-normalized */,
                              3*sizeof(float)   /* stride length */,
                              nullptr           /* attrib offset */);
        
        /* Indices */
        uint32_t indices[3] = {0, 1, 2};
        
        /* Index Buffer (IB) */
        m_IndexBuffer.reset(IndexBuffer::Create(indices, 3 /* sizeof(indices)/sizeof(uint32_t) */));
        
        /* Wrinting Shaders */
        std::string vertexSrc = R"(
            
                #version 330 core
                
                layout(location = 0) in vec3 a_Position;
                
                out vec3 v_Position;
                
                void main()
                {
                    v_Position = a_Position;
                    gl_Position = vec4(a_Position, 1.0);
                }
                
            )";
        
        std::string fragmentSrc = R"(
            
                #version 330 core
                
                layout(location = 0) out vec4 color;
                
                in vec3 v_Position;
                
                void main()
                {
                    color = vec4(v_Position * 0.5 + 0.5, 1.0);
                }
                
            )";
        
        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
    }
    
    Application::~Application()
    {

    }
    
    void Application::Run()
    {
        m_isRunning = true;
        LOG_INFO("Sandbox App is now running...");
        
        while(m_isRunning)
        {
            glClearColor(0.02f, 0.31f, 0.7f, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            
            m_Shader->Bind();
            glBindVertexArray(m_VA);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
            
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            
            p_Window->OnUpdate();
        }
    }
    
    void Application::OnEvent(Event& event)
    {
        bool handled = false;
        
        /* Log Event */
        //LOG_TRACE(event.ToString());
        
        /* Dispatch Event */
        handled = EventDispatcher::StaticDispatch<Application>(event, this);
        
        if (!handled)
        {
            //LOG_WARN("Event could not be dispatched by the EventDispatcher");
        }
        
        /* it is casted to a Layer */
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            /**
             *  We start from the TOP View (last drawn Layer)
             *  and we go up to first drawn Layer
             */
            (*--it)->OnEvent(event);    /* We try to dispatch de Layer to the upmost Layer */
            
            /**
             * We break on the Layer on which the event belongs to (was handled)
             */
            if (event.handled)
            {
                break;
            }
        }
    }
    
    bool Application::OnWindowClose()
    {
        LOG_WARN("Performing a graceful shutdown sequence...");
        m_isRunning = false;
        
        return true;    /* Event was handled */
    }
    
    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    
    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }
}