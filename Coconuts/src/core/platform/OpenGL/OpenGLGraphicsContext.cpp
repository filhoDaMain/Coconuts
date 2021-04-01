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

#include "OpenGLGraphicsContext.h"
#include <coconuts/Logger.h>
#include <glad/glad.h>

namespace Coconuts
{
    
    OpenGLGraphicsContext::OpenGLGraphicsContext(GLFWwindow* window)
        :   windowHandle(window)
    {
    }

    void OpenGLGraphicsContext::PreInitHints(unsigned int platform)
    {
        switch (platform)
        {
            case TargetPlatform::Platform_MacOS:
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//3
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);//2
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                
                LOG_TRACE("OpenGLGraphicsContext - Pre Init hints set for MacOS platform");
                break;
            }

            case TargetPlatform::Platform_GNU:
            {
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//3
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);//2
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
                
                LOG_TRACE("OpenGLGraphicsContext - Pre Init hints set for GNU platform");
                break;
            }
            
            default:
            {
                LOG_ERROR("PreInitHints - Unknown platform {}. No Hints set.", platform);
            }
        }
        
    }
    
    void OpenGLGraphicsContext::Init()
    {
        int rc;
        
        glfwMakeContextCurrent(windowHandle);
        LOG_TRACE("A GLFW Window was created and set to current context"); 
        
        rc = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if (rc > 0)
        {
            LOG_DEBUG("GLAD initialized");
        }
        else
        {
            LOG_CRITICAL("Failed to initialize GLAD. Exiting...");
            exit(1);
        }
        
        LOG_TRACE("Coconuts Graphics info:");
        LOG_TRACE("* Using OpenGL ver:  {}", glGetString(GL_VERSION));
        LOG_TRACE("* From vendor:       {}", glGetString(GL_VENDOR));
        LOG_TRACE("* Renderer:          {}", glGetString(GL_RENDERER));
        LOG_TRACE("* Shading Language:  {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
    }
    
    void OpenGLGraphicsContext::SwapBuffers()
    {
        /* Swap front and back buffers */
        glfwSwapBuffers(windowHandle);
    }
    
}