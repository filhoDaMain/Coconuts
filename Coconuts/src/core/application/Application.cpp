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
#include <coconuts/Logger.h>
#include <coconuts/Renderer.h>
#include <coconuts/graphics/Renderer2D.h>
#include <coconuts/time/Timestep.h>
#include <GLFW/glfw3.h>
#include <coconuts/editor_gui/GUILayer.h>
#include <coconuts/types.h>
#include <coconuts/AssetManager.h>
#include <coconuts/SceneManager.h>
#include "AppManager.h"


namespace Coconuts
{
    /* Singleton Pattern */
    Application* Application::s_Instance = nullptr;
    
    Application::Application(const std::string& appname)
    : m_AppName(appname)
    {
        /* Assert that the Singleton Pattern is respected */
        if (s_Instance != nullptr)
        {
            LOG_CRITICAL("Crash! Application already exists.");
            exit(1);
        }
        
        s_Instance = this;
      
        LOG_DEBUG("App created!");
        
        p_Window = std::unique_ptr<Window>(Window::Create( WindowProperties(appname) ));
        
        /* Initialize abstracted Renderer */
        Renderer::Init();
        Renderer2D::Init();
        LOG_DEBUG("High-Level Coconuts Renderer initialized");
        
        /* VSync enable/disable */
        p_Window->SetVSync(true);
        
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
        p_Window->SetEventCallback(BIND_EVENT_FUNCTION(Application::OnEvent));
        
        /* Pre-alloc AssetManager hash tables */
        AssetManager::Init();
        
        /* Init SceneManager's singleton */
        p_SceneManager = std::unique_ptr<SceneManager>(new SceneManager());
    }
    
    Application::~Application()
    {

    }
    
    void Application::Run()
    {
        m_isRunning = true;
        LOG_INFO("App is now running...");
        
        while(m_isRunning)
        {
            float time = (float) glfwGetTime(); //TODO: Make this a platform dependent func call
            Timestep timestep = time - m_LastFrameTime;
            m_LastFrameTime = time;
            
            for (Layer* layer : m_LayerStack)
            {
                /* GUI Layer */
                if (layer->IsGUI() == true)
                {
                        Editor::GUILayer* gui = dynamic_cast<Editor::GUILayer*>(layer);
                        gui->Begin();
                        gui->OnUpdate(timestep);
                        gui->End();
                }
                    
                /* Normal Layer */
                else
                {   
                    /* Only update layers if main window is not minimized */
                    if (!m_isMainWindMinimized)
                    {
                        layer->OnUpdate(timestep);
                    }
                } 
            }
            
            p_Window->OnUpdate();
        }
    }
    
    void Application::OnEvent(Event& event)
    {
        bool handled = false;
        
        /* Log Event */
        //LOG_TRACE(event.ToString());
        
        /* Dispatch Window Related events to the appropriate Applicattion::Callback() */
        
        /**
         * Dispatch Window Related events to the appropriate
         * Application::OnEventCallback()
         */
        EventDispatcher dispatcher(event);
        
        // Window Close
        dispatcher.Dispatch<WindowEvent::WindowClose>(BIND_EVENT_FUNCTION(Application::OnWindowClose));
        
        // Window Resize
        dispatcher.Dispatch<WindowEvent::WindowResize>(BIND_EVENT_FUNCTION(Application::OnWindowResize));
        
        // Window Minimize
        dispatcher.Dispatch<WindowEvent::WindowMinimize>(BIND_EVENT_FUNCTION(Application::OnWindowMinimize));
        
        /**
         * Dispatch the event (Window Event or not) to the respective
         * OnEvent() callback on which the layer has occured
         */
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
        {
            /**
             *  We start from the TOP View (last drawn Layer)
             *  and we go down to the first drawn Layer
             */
            (*--it)->OnEvent(event);    /* We try to dispatch the event to the upmost Layer */
            
            /**
             * We break on the Layer on which the event belongs to (was handled)
             */
            if (event.handled)
            {
                break;
            }
        }
    }
    
    bool Application::OnWindowClose(WindowEvent::WindowClose& event)
    {
        LOG_WARN("Performing a graceful shutdown sequence...");
        m_isRunning = false;
        
        return true;    /* Event was handled. Stop the event propagation */
    }
    
    bool Application::OnWindowResize(WindowEvent::WindowResize& event)
    {
        LOG_TRACE("Window Resize: {} x {}", event.GetWidth(), event.GetHeight());
        
        Renderer::UpdateRenderScreenSize(event.GetWidth(), event.GetHeight());
        return false;    /* Let event propagate to lower layers */
    }
    
    bool Application::OnWindowMinimize(WindowEvent::WindowMinimize& event)
    {
        if (event.IsMinimized() == true)
        {
            LOG_TRACE("Window Minimized: status = true (minimized)");
            m_isMainWindMinimized = true;
        }
        
        else
        {
            LOG_TRACE("Window Minimized: status = false");
        }
        
        return true;    /* Stop the event propagation */
    }
    
    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
        layer->OnPostAttach();
    }
    
    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
        overlay->OnPostAttach();
    }
    
    

    //static
    void AppManagerProxy::LoadRuntimeConfig()
    {
        std::string filepath = "dummy";
        auto& app = Application::GetInstance();
        std::string appName = app.GetApplicationName();
        
        /* Get filepath = appName.meta or appName.ccnproj */
        /* Call LoadRuntimeConfig(filepath )*/
        
        if (!AppManager::LoadRuntimeConfig(filepath))
        {
            /* Deal with failure */
            //TODO
        }
    }
    
    //static
    void AppManagerProxy::LoadRuntimeConfig(std::string& filepath)
    {
        /* Just handover */
        if (!AppManager::LoadRuntimeConfig(filepath))
        {
            /* Deal with failure */
            //TODO
        }
        
    }
    
}