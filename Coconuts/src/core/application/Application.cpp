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
#include <coconuts/FileSystem.h>
#include "AppManager.h"


namespace Coconuts
{
    /* Singleton Pattern */
    Application* Application::s_Instance = nullptr;
    
    Application::Application(std::shared_ptr<Window> window, const std::string& appname)
    : m_AppName(appname), m_GameLayer()
    {
        /* Assert that the Singleton Pattern is respected */
        if (s_Instance != nullptr)
        {
            LOG_CRITICAL("Crash! Application already exists.");
            exit(1);
        }
        
        s_Instance = this;
        p_Window = window;
        
        /* Initialize abstracted Renderer */
        Renderer::Init();
        Renderer2D::Init();
        LOG_DEBUG("High-Level Coconuts Renderer initialized");
        
        /* VSync enable/disable */
        p_Window->SetVSync(true);
        
        /* Pre-alloc AssetManager hash tables */
        AssetManager::Init();

        /* Push main game layer */
        PushLayer(static_cast<Layer*>(&m_GameLayer));

        LOG_INFO("Game App created!");
    }
    
    Application::~Application()
    {

    }
    
    void Application::Run()
    {
        float time = (float) glfwGetTime(); //TODO: Make this a platform dependent func call
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;

        for (Layer* layer : m_LayerStack)
        {
            layer->OnUpdate(timestep);
        }
    }

    void Application::OnEvent(Event& event)
    {
        bool handled = false;

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
        std::string filepath_ccnproj = FileSystem::GetRuntimeConfDirPath();
        std::string filepath_meta = FileSystem::GetRuntimeConfDirPath();
        std::string appName = Application::GetInstance().GetApplicationName();
        
        filepath_meta += appName + "." + Parser::FILE_EXTENSIONS::METABINARY_FILE_EXT;
        filepath_ccnproj += appName + "." + Parser::FILE_EXTENSIONS::YAML_PROJECT_FILE_EXT;
        
        /**
         * Try to load meta binary first.
         * If it fails, try to load from yaml configuration file.
         */
        if (!AppManager::LoadRuntimeConfig(filepath_meta))
        {
            LOG_ERROR("Meta binary file failed to load!");
            LOG_DEBUG("Trying to load YAML configuration file instead...");
            
            if (!AppManager::LoadRuntimeConfig(filepath_ccnproj))
            {
                LOG_CRITICAL("Failed to initialize application with user defined configuration!");
                return;
            }
        }
    }
    
    //static
    void AppManagerProxy::LoadRuntimeConfig(std::string& filepath)
    {
        /* Just handover */
        if (!AppManager::LoadRuntimeConfig(filepath))
        {
            LOG_CRITICAL("Failed to initialize application with user defined configuration!");
            return;
        }
        
    }
    
}