/*
 * Copyright 2021 Andre Temprilho
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
#ifndef GNUWINDOW_H
#define GNUWINDOW_H

#include <cstdint>
#include <coconuts/window_system/Window.h>
#include <GLFW/glfw3.h>
#include <coconuts/graphics/GraphicsContext.h>
#include <coconuts/EventSystem.h>

namespace Coconuts
{
    class GNUWindow : public Window
    {
    public:
        GNUWindow(const WindowProperties& props);
        ~GNUWindow();
            
        void OnUpdate() override;
        
        void SetEventCallback(const EventCallbackFunction& callbackFn) override
        {
            /* Make our GNUWindow callback for events point to callbackFn */
            m_WindowData.eventCallback = callbackFn;
        }
        
        virtual void EventTrigger(Event& event) override
        {
            m_WindowData.eventCallback(event);
        }
        
        bool InitWindowManagerCallbacks(const char* library) override;
        uint32_t GetWidth() const override { return m_WindowData.width; }
        uint32_t GetHeight() const override { return m_WindowData.height; }
        
        void SetVSync(bool enable) override;
        bool IsVsyncEnabled() const override;
        
        inline void* GetNativeWindow() const override { return p_glfwWindow; }
            
    private:
        void Init(const WindowProperties& props);
        void ShutDown();
        
        struct GNUWindowData
        {
            std::string title;
            uint32_t width, height;
            bool VSync;
            
            /* Called to dispatch any GNUWindow Event */
            EventCallbackFunction eventCallback;
        };   
        GNUWindowData m_WindowData;
        
        GLFWwindow* p_glfwWindow;
        
        GraphicsContext* graphicsContext;
    };
}

#endif /* GNUWINDOW_H */

