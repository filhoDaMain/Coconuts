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
#ifndef MACWINDOW_H
#define MACWINDOW_H

#include <coconuts/window_system/Window.h>
#include <GLFW/glfw3.h>

namespace Coconuts
{
    class MacWindow : public Window
    {
    public:
        MacWindow(const WindowProperties& props);
        ~MacWindow();
            
        void OnUpdate() override;
            
        unsigned int GetWidth() const override { return m_WindowData.width; }
        unsigned int GetHeight() const override { return m_WindowData.height; }
            
    private:
        void Init(const WindowProperties& props);
        void ShutDown();
        
        struct MacWindowData
        {
            std::string title;
            unsigned int width, height;
        };   
        MacWindowData m_WindowData;
        
        GLFWwindow* p_glfwWindow;
    };
}

#endif /* MACWINDOW_H */

