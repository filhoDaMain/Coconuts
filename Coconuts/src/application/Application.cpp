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

#include <coconuts/core.h>
#include <coconuts/Logger.h>
#include <coconuts/EventSystem.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;

namespace Coconuts
{
    Application::Application()
    {
        LOG_INFO("Sandbox App created!");
        
        /* Initialize the library */
        if (!glfwInit())
        {
            LOG_CRITICAL("GLFW was not properly initialized. Closing app...");
            exit(1);
        }
        LOG_TRACE("GLFW initialized");
        
        const char* glsl_version = "#version 150";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Coconuts", NULL, NULL);
        if (!window)
        {
            LOG_CRITICAL("Window was not created. Closing app...");
            glfwTerminate();
            exit(1);
        }
        LOG_TRACE("Window created");
        glfwSwapInterval(1);
        
        /* Make the window's context current */
        glfwMakeContextCurrent(window);
    }
    
    Application::~Application()
    {

    }
    
    void Application::Run()
    {
        LOG_INFO("Sandbox App is now running...");
        
        /* Loop until the user closes the window */
        LOG_DEBUG("Entering Rendering loop...");
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
        LOG_WARN("Rendering loop exited because window was closed");
        glfwTerminate();
    }
}