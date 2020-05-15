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

#include "MacWindow.h"
#include <coconuts/Logger.h>

namespace Coconuts
{   
    static bool s_GLFWinitialized = false;
    
    Window* Window::Create(const WindowProperties& props)
    {
        return new MacWindow(props);
    }
    
    MacWindow::MacWindow(const WindowProperties& props)
    {
        Init(props);
    }
    
    MacWindow::~MacWindow()
    {
        ShutDown();
    }
    
    /* Private - Shall only be called from MacWindow() constructor */
    void MacWindow::Init(const WindowProperties& props)
    {
        int rc;
        m_WindowData.title = props.m_Title;
        m_WindowData.width = props.m_Width;
        m_WindowData.height = props.m_Height;
        
        if (!s_GLFWinitialized)
        {
            rc = glfwInit();
            if (rc == GLFW_TRUE)
            {
                s_GLFWinitialized = true;
                LOG_DEBUG("GLFW initialized");
            }
            else
            {
                LOG_CRITICAL("Failed to initialize GLFW. Exiting with {} ...", rc);
                exit(rc);
            }
        }
         
        /* Create a windowed mode window and its OpenGL context */
        m_Window = glfwCreateWindow((int) m_WindowData.width, (int) m_WindowData.height,
                                    m_WindowData.title.c_str(), nullptr, nullptr);
        
        if (m_Window == NULL)
        {
            LOG_CRITICAL("Failed to create a GLFW Window. Exiting...");
            exit(1);
        }
        
        /* Make the window's context current */
        glfwMakeContextCurrent(m_Window);
        
        LOG_TRACE("A GLFW Window was created and set to current context");
    }
    
    /* Private - Shall only be called from ~MacWindow() deconstructor */
    void MacWindow::ShutDown()
    {
        glfwDestroyWindow(m_Window);
    }
    
    void MacWindow::OnUpdate()
    {
        /* Swap front and back buffers */
        glfwSwapBuffers(m_Window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}