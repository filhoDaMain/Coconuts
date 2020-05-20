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
#include <coconuts/window_system/Window.h>
#include <coconuts/EventSystem.h>
#include <coconuts/Layer.h>
#include <coconuts/layer_system/LayerStack.h>

namespace Coconuts
{
    
    class Application
    {   
    public:
        Application();
        ~Application();
        
        void Run(void);
        void OnEvent(Event& event);
        bool OnWindowClose(void);
        
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        
    private:
        bool m_isRunning = false;
        
        /* Generic Window tied to a platform dependent Window sublclass */
        std::unique_ptr<Window> p_Window;
        
        LayerStack m_LayerStack;
    };
    
    extern Application* CreateApplication();
    
}

#endif /* APPLICATION_H */
