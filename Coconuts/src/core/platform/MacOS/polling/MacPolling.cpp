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

#include "MacPolling.h"
#include <coconuts/Application.h>
#include <GLFW/glfw3.h>

namespace Coconuts
{
    /* Singleton instance */
    Polling* Polling::s_Instance = new MacPolling();
    
    /* Key */
    bool MacPolling::IsKeyPressedImpl(int keyCode)
    {
        auto glfwWindow = static_cast<GLFWwindow*>
                (Application::GetInstance().GetWindow().GetNativeWindow());
        
        auto state = glfwGetKey(glfwWindow, keyCode);
        
        return (state == GLFW_PRESS || state == GLFW_REPEAT);
    }
    
    /* Mouse */
    bool MacPolling::IsMouseButtonPressedImpl(int keyCode)
    {
        auto glfwWindow = static_cast<GLFWwindow*>
                (Application::GetInstance().GetWindow().GetNativeWindow());
        
        auto state = glfwGetMouseButton(glfwWindow, keyCode);
        
        return (state == GLFW_PRESS);
    }
    
    double MacPolling::GetMouseXImpl()
    {
        auto glfwWindow = static_cast<GLFWwindow*>
                (Application::GetInstance().GetWindow().GetNativeWindow());
        
        double xPos, yPos;
        glfwGetCursorPos(glfwWindow, &xPos, &yPos);
        return xPos;
    }
    
    double MacPolling::GetMouseYImpl()
    {
        auto glfwWindow = static_cast<GLFWwindow*>
                (Application::GetInstance().GetWindow().GetNativeWindow());
        
        double xPos, yPos;
        glfwGetCursorPos(glfwWindow,  &xPos, &yPos);
        return yPos;
    }
}
