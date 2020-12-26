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

#include <coconuts/editor.h>
#include <coconuts/Application.h>
#include <coconuts/EventSystem.h>
#include <coconuts/Logger.h>
#include <GLFW/glfw3.h>
#include <coconuts/types.h>


using namespace Coconuts::Editor;

#include <coconuts/editor_gui/GUILayer.h>
#include <imgui/examples/imgui_impl_opengl3.h>
#include <imgui/examples/imgui_impl_glfw.h>

GUILayer::GUILayer()    : Layer("GUILayer")
{
}

GUILayer::~GUILayer()
{   
}

void GUILayer::OnAttach()
{
    /**
     * Referece:
     *  imgui/examples/example_glfw_opengl3/main.cpp
     */
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    
    
    /* Coconuts Application Native Window - GLFWwindow* */
    /* --------------------------------------------------------------------------- */
    Application& app = Application::GetInstance();
    GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());
    /* --------------------------------------------------------------------------- */
    
    
    /**
     * Referece:
     *  imgui/examples/example_glfw_opengl3/main.cpp
     */
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window /* GLFWwindow* */, true);
    ImGui_ImplOpenGL3_Init("#version 150" /* chosen by me */);
    
    
    LOG_DEBUG("GUI Layer attached");
}

void GUILayer::OnPostAttach()
{
    
}

void GUILayer::OnDetach()
{
    /**
     * Referece:
     *  imgui/examples/example_glfw_opengl3/main.cpp
     */
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUILayer::OnUpdate(Timestep ts)
{
    // override on sandbox app
}

void GUILayer::Begin()
{
    /**
     * Referece:
     *  imgui/examples/example_glfw_opengl3/main.cpp
     */
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUILayer::End()
{
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::GetInstance();
    io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());
    
    /**
     * Referece:
     *  imgui/examples/example_glfw_opengl3/main.cpp
     */
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}