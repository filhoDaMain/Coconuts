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
#include <cstring>
#include "OpenGLGraphicsContext.h"

namespace Coconuts
{   
    static bool s_GLFWinitialized = false;
    
    static void GLFWErrorCallback(int error, const char* description)
    {
        LOG_ERROR("GLFW Error {:X}: {}", error, description);
    }
    
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
                glfwSetErrorCallback(GLFWErrorCallback);
                LOG_DEBUG("GLFW initialized");
            }
            else
            {
                LOG_CRITICAL("Failed to initialize GLFW. Exiting with {} ...", rc);
                exit(rc);
            }
        }
                
        /* Some necessary Hints for platform MacOS before window creation */
        OpenGLGraphicsContext::PreInitHints(TargetPlatform::Platform_MacOS);
        
        /* Create a windowed mode window and its OpenGL context */
        p_glfwWindow = glfwCreateWindow((int) m_WindowData.width, (int) m_WindowData.height,
                                    m_WindowData.title.c_str(), nullptr, nullptr);
        
        if (p_glfwWindow == NULL)
        {
            LOG_CRITICAL("Failed to create a GLFW Window. Exiting...");
            exit(1);
        }
        
        /* Create Graphics Context */
        graphicsContext = new OpenGLGraphicsContext(p_glfwWindow);
        graphicsContext->Init();
        
        /* Make the window's context current */
        glfwSetWindowUserPointer(p_glfwWindow, &m_WindowData);
    }
    
    bool MacWindow::InitWindowManagerCallbacks(const char* library)
    {
        if (strcmp(library, "GLFW") == 0)
        {
            LOG_TRACE("Initializing GLFW callbacks...");
            
            glfwSetInputMode(p_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            
            /* Window Close */
            glfwSetWindowCloseCallback(p_glfwWindow, [](GLFWwindow* window)
            {
                /**
                 * Get the user pointer associated to the GLFW Window on which
                 * this event has occured
                 */
                MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
            
                /* Create the associated Coconuts Event for this kind of event */
                WindowEvent::WindowClose winCloseEvent;
            
                /* Dispatch this event to the callback function associated with this window */
                thisWinData.eventCallback(winCloseEvent);
            });
             LOG_TRACE("* <WindowCloseCallback>  initialized");
            
            /* Window (Re)size */
            glfwSetWindowSizeCallback(p_glfwWindow, [](GLFWwindow* window, int width, int height)
            {
                MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
            
                /* Update the window data */
                thisWinData.width = width; 
                thisWinData.height = height;
            
                /* Create the associated Coconuts Event for this kind of event */
                WindowEvent::WindowResize winResizeEvent(width, height);
            
                /* Dispatch this event to the callback function associated with this window */
                thisWinData.eventCallback(winResizeEvent);
            });
            LOG_TRACE("* <WindowSizeCallback>   initialized");
            
            /* Window Minimize */
            glfwSetWindowIconifyCallback(p_glfwWindow, [](GLFWwindow* window, int iconified)
            {
                 /**
                 * Get the user pointer associated to the GLFW Window on which
                 * this event has occured
                 */
                MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
            
                /* Create the associated Coconuts Event for this kind of event */
                bool isMinimized = (iconified == GLFW_TRUE) ? true : false;
                
                WindowEvent::WindowMinimize winMinimizeEvent(isMinimized);
            
                /* Dispatch this event to the callback function associated with this window */
                thisWinData.eventCallback(winMinimizeEvent);
            });
            LOG_TRACE("* <WindowSizeCallback>   initialized");
            
            /* Keyboard Inputs */
            glfwSetKeyCallback(p_glfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
            
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        InputKeyEvent::KeyPress keyPress(key, 0);
                        thisWinData.eventCallback(keyPress);
                        break;
                    }
                    
                    case GLFW_RELEASE:
                    {
                        InputKeyEvent::KeyRelease keyRelease(key);
                        thisWinData.eventCallback(keyRelease);
                        break;
                    }
                        
                    case GLFW_REPEAT:
                    {
                        InputKeyEvent::KeyPress keyPress(key, 1);
                        thisWinData.eventCallback(keyPress);
                        break;
                    }
                }
            });
            LOG_TRACE("* <KeyCallback>          initialized     [Input: Keyboard]");
            
            /* Mouse Inputs */
            glfwSetMouseButtonCallback(p_glfwWindow, [](GLFWwindow* window, int button, int action, int mods)
            {
                MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
            
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        InputMouseEvent::MouseButtonPress buttonPress(button);
                        thisWinData.eventCallback(buttonPress);
                        break;
                    }
                    
                    case GLFW_RELEASE:
                    {
                        InputMouseEvent::MouseButtonRelease buttonRelease(button);
                        thisWinData.eventCallback(buttonRelease);
                        break;
                    }
                }
            });
            LOG_TRACE("* <MouseButtonCallback>  initialized     [Input:    Mouse]");
            
            glfwSetCursorPosCallback(p_glfwWindow, [](GLFWwindow* window, double xpos, double ypos)
            {
                MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
                
                InputMouseEvent::MouseCursorMove cursorMove(xpos, ypos);
                thisWinData.eventCallback(cursorMove);
            });
            LOG_TRACE("* <CursorPosCallback>    initialized     [Input:    Mouse]");
            
            glfwSetScrollCallback(p_glfwWindow, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                MacWindowData &thisWinData = *((MacWindowData*)glfwGetWindowUserPointer(window));
                
                InputMouseEvent::MouseScroll scroll(xoffset, yoffset);
                thisWinData.eventCallback(scroll);
            });
            LOG_TRACE("* <ScrollCallback>       initialized     [Input:    Mouse]");
            
            return true;
        }
        
        return false;   /* unknown library */
    }
    
    /* Private - Shall only be called from ~MacWindow() deconstructor */
    void MacWindow::ShutDown()
    {
        glfwDestroyWindow(p_glfwWindow);
    }
    
    void MacWindow::OnUpdate()
    {
        graphicsContext->SwapBuffers();
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    void MacWindow::SetVSync(bool enable)
    {
        if (enable)
        {
            glfwSwapInterval(1);
            LOG_DEBUG("VSync enabled");
        }
        
        else    // disable 
        {
            glfwSwapInterval(0);
            LOG_DEBUG("VSync disabled");
        }
        
        m_WindowData.VSync = enable;
    }
    
    bool MacWindow::IsVsyncEnabled() const
    {
        return m_WindowData.VSync;
    }
}