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
#include <coconuts/EventSystem.h>
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
        LOG_TRACE("MacWindow objects freed");
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
        p_glfwWindow = glfwCreateWindow((int) m_WindowData.width, (int) m_WindowData.height,
                                    m_WindowData.title.c_str(), nullptr, nullptr);
        
        if (p_glfwWindow == NULL)
        {
            LOG_CRITICAL("Failed to create a GLFW Window. Exiting...");
            exit(1);
        }
        
        /* Make the window's context current */
        glfwSetWindowUserPointer(p_glfwWindow, &m_WindowData);
        glfwMakeContextCurrent(p_glfwWindow);
        LOG_TRACE("A GLFW Window was created and set to current context");
        
        
        /* Set GLFW Callbacks */
        
        /* Window Close */
        glfwSetWindowCloseCallback(p_glfwWindow, [](GLFWwindow* window)
        {
            /**
             * Get the user pointer associated to the GLFW Window on which
             * this event has occured
             */
            MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
            
            /* Create the associated Coconuts Event for this kind of event */
            Events::WindowClose winCloseEvent;
            
            /* Dispatch this event to the callback function associated with this window */
            thisWinData.eventCallback(winCloseEvent);
        });
        
        /* Window (Re)size */
        glfwSetWindowSizeCallback(p_glfwWindow, [](GLFWwindow* window, int width, int height)
        {
            /**
             * Get the user pointer associated to the GLFW Window on which
             * this event has occured
             */
            MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
            
            /* Update the window data */
            thisWinData.width = width; 
            thisWinData.height = height;
            
            /* Create the associated Coconuts Event for this kind of event */
            Events::WindowResize winResizeEvent(width, height);
            
            /* Dispatch this event to the callback function associated with this window */
            thisWinData.eventCallback(winResizeEvent);
        });
              
    }
    
    /* Private - Shall only be called from ~MacWindow() deconstructor */
    void MacWindow::ShutDown()
    {
        glfwDestroyWindow(p_glfwWindow);
    }
    
    void MacWindow::OnUpdate()
    {
        /* Swap front and back buffers */
        glfwSwapBuffers(p_glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }
}