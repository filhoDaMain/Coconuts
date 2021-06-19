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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <string>
#include <coconuts/window_system/Window.h>
#include <coconuts/EventSystem.h>
#include <coconuts/Layer.h>
#include <coconuts/layer_system/LayerStack.h>

namespace Coconuts
{
    class SceneManager; // forward declared
    
    /**
     * Singleton
     */
    class Application
    {   
    public:
        Application(const std::string& appname = "Application");
        ~Application();
        
        inline static Application& GetInstance() { return *s_Instance; }
        inline Window& GetWindow() const { return *p_Window; }
        
        void Run(void);
        void OnEvent(Event& event);
        bool OnWindowClose(WindowEvent::WindowClose& event);
        bool OnWindowResize(WindowEvent::WindowResize& event);
        bool OnWindowMinimize(WindowEvent::WindowMinimize& event);
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        
        std::string GetApplicationName() { return m_AppName; }
        
    private:
        std::string m_AppName;
        
        /* Single instance */
        static Application* s_Instance;
        
        std::unique_ptr<SceneManager> p_SceneManager;
        bool m_isRunning = false;
        bool m_isMainWindMinimized = false;
        
        /* Generic Window tied to a platform dependent Window sublclass */
        std::unique_ptr<Window> p_Window;
        
        LayerStack m_LayerStack;
        
        float m_LastFrameTime = 0.0f;
    };
    
    
    class AppManagerProxy
    {
    public:
        static void LoadRuntimeConfig();
        static void LoadRuntimeConfig(std::string& filepath);
    };
    
    extern Application* CreateApplication();
    
}

#endif /* APPLICATION_H */

