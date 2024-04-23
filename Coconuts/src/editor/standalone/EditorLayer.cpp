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

#include "EditorLayer.h"
#include "fonts/Fonts.h"
#include <cstdint>
#include <coconuts/FileSystem.h>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <coconuts/Application.h>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

namespace Coconuts
{
    
    namespace {
    static std::string s_layoutFile;

    namespace Parser
    {
        namespace DEFAULTS
        {
            constexpr auto EDITOR_APP_LAYOUT_FILE = "layout.ini";
        }
    }
    } //namespace
    
    void EditorLayer::Init(void)
    {
        m_GameLayerPtr = static_cast<GameLayer*>(&Application::GetInstance().GetGameLayer());
        PreInit();
        PostInit();
    }

    void EditorLayer::Draw()
    {
        Begin();
        RenderGUI();
        End();
    }

    void EditorLayer::PreInit(void)
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

        /* Editor Application Native Window - GLFWwindow* */
        /* --------------------------------------------------------------------------- */
        GLFWwindow* window = static_cast<GLFWwindow*>(m_Window.GetNativeWindow());
        /* --------------------------------------------------------------------------- */

        /**
         * Referece:
         *  imgui/examples/example_glfw_opengl3/main.cpp
         */
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window /* GLFWwindow* */, true);
        ImGui_ImplOpenGL3_Init("#version 150" /* chosen by me */);

        LOG_DEBUG("EditorLayer initialized");
    }

    void EditorLayer::PostInit(void)
    {
        /* Init flags */
        m_ShowPopUp_CreateSprite = false;
        m_ShowPopUp_ImportTexture2D = false;
        m_ShowPopUp_LoadProject = false;
        m_ShowPopUp_SaveProject = false;

        /* Set Style */
        SetCustomGUIStyle();

        /* Menus */
        m_AssetsMenu.Init(&m_ShowPopUp_ImportTexture2D, &m_ShowPopUp_CreateSprite);
        m_FileMenu.Init(m_GameLayerPtr, &m_ShowPopUp_LoadProject, &m_ShowPopUp_SaveProject);
        m_EntityMenu.Init(m_GameLayerPtr);

        /* Panels */
        m_ViewportPanel.Init(m_GameLayerPtr);
        m_ComponentInspectorPanel.Init();
        m_SceneOverviewPanel.Init(m_GameLayerPtr, &m_ComponentInspectorPanel);
        m_AssetInspectorPanel.Init();
        m_AssetsPanel.Init(&m_AssetInspectorPanel);

        /* PopUps */
        m_ImportTexture2dPopUp.Init(&m_ShowPopUp_ImportTexture2D);
        m_CreateSpritePopUp.Init(&m_ShowPopUp_CreateSprite);
        m_LoadProjectPopUp.Init(m_GameLayerPtr, &m_ComponentInspectorPanel, &m_ShowPopUp_LoadProject);
        m_SaveProjectPopUp.Init(&m_ShowPopUp_SaveProject);
    }

    void EditorLayer::Begin(void)
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

    void EditorLayer::End(void)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2((float)m_Window.GetWidth(), (float)m_Window.GetHeight());

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
    
    void EditorLayer::RenderGUI(void)
    {
        /**
         * SET UP DOCK SPACE
         * 
         * reference:
         *      imgui_demo.cpp:
         *          ShowExampleAppDockSpace() 
         */
        
        static bool dockspaceOpen = true;
        
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        }
        
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        
        if (!opt_padding)
        {
            ImGui::PopStyleVar();
        }

        if (opt_fullscreen)
        {
            ImGui::PopStyleVar(2);
        }

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        
        
        /*  M E N U   B A R  */
        
        if (ImGui::BeginMenuBar())
        {
            /* File Menu */
            m_FileMenu.Draw();
            
            /* Entity Menu */
            m_EntityMenu.Draw();
            
            /* Assets Menu */
            m_AssetsMenu.Draw();
            
            ImGui::EndMenuBar();
        }
        
        
        /*  P O P   U P S  */
        
        if (m_ShowPopUp_ImportTexture2D)
        {
            /* Import Texture 2D (Triggered from AssetsMenu) */
            m_ImportTexture2dPopUp.Draw();
        }
        
        if (m_ShowPopUp_CreateSprite)
        {
            /* Create Sprite (Triggered from AssetsMenu) */
            m_CreateSpritePopUp.Draw();
        }
        
        if (m_ShowPopUp_LoadProject)
        {
            /* Load Project (Triggered from FileMenu) */
            m_LoadProjectPopUp.Draw();
        }
        
        if (m_ShowPopUp_SaveProject)
        {
            /* Save Project (Triggered from FileMenu) */
            m_SaveProjectPopUp.Draw();
        }
        
        
        /*  P A N E L S  */
        
        /* Viewport Panel */
        m_ViewportPanel.LiveUpdate();
        
        /* Statistics Panel */
        m_StatisticsPanel.Draw(Renderer2D::GetStatistics());

        /* Scene Overview Panel */
        m_SceneOverviewPanel.Draw();
        
        /* Assets Panel */
        m_AssetsPanel.Draw();
        
        /* Component Inspector */
        m_ComponentInspectorPanel.Draw();
        
        /* Asset Inspector */
        m_AssetInspectorPanel.Draw();
        
        //DEBUG:
        //ImGui::ShowDemoWindow();
        
        // ---------------------------------------------------------------------------
        // DOCK SPACE END
    }
    
    void EditorLayer::SetCustomGUIStyle()
    {
        /* Set Font and font size */
        SetDefaultFontTTF(Fonts::Lato::Regular.c_str(), 16.0f);
        
        /* Set custom dark theme */
        SetDarkThemeColors();
        
        /* Load last saved Editor App layout */
        LoadLayout();
    }
    
    void EditorLayer::SetDarkThemeColors()
    {
        LOG_TRACE("Using Dark Theme for Editor GUI");
         
        ImVec4* colors = ImGui::GetStyle().Colors;
        ImVec4 BLACK = {1.0f, 1.0f, 1.0f, 1.0f};
        ImVec4 MAGENTA = {1.0f, 0.f, 1.0f, 1.0f};
        
        /* Init background */
        colors[ImGuiCol_DockingEmptyBg]         = BLACK;
        
        /* Window */
        colors[ImGuiCol_WindowBg]               = ImVec4{0.102f, 0.105f, 0.109f, 1.0f}; // Panel background
        colors[ImGuiCol_ChildBg]                = ImVec4{0.102f, 0.105f, 0.109f, 1.0f}; // Panel background
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4{0.102f, 0.105f, 0.109f, 1.0f}; // Panel background
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4{0.102f, 0.105f, 0.109f, 1.0f}; // Panel background
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4{0.102f, 0.105f, 0.109f, 1.0f}; // Panel background
        
        /* Menu Bar */
        colors[ImGuiCol_MenuBarBg]              = ImVec4{0.081f, 0.083f, 0.086f, 1.0f}; 
                
        /* Header */
        colors[ImGuiCol_Header]                 = ImVec4{0.195f, 0.203f, 0.206f, 1.0f}; // Selected
        colors[ImGuiCol_HeaderHovered]          = ImVec4{0.235f, 0.239f, 0.240f, 1.0f}; // Hover
        colors[ImGuiCol_HeaderActive]           = ImVec4{0.297f, 0.297f, 0.297f, 1.0f}; // Click
        
        /* Title */
        colors[ImGuiCol_TitleBg]                = ImVec4{0.130f, 0.137f, 0.143f, 1.0f}; // Normal
        colors[ImGuiCol_TitleBgActive]          = ImVec4{0.182f, 0.189f, 0.189f, 1.0f}; // Selected
        colors[ImGuiCol_TitleBgCollapsed]       = MAGENTA;//ImVec4{0.235f, 0.239f, 0.240f, 1.0f};
                
        /* Text */        
        colors[ImGuiCol_Text]                   = ImVec4{0.914f, 0.917f, 0.920f, 1.0f}; // Normal
        colors[ImGuiCol_TextDisabled]           = ImVec4{0.347f, 0.351f, 0.354f, 1.0f}; // Disabled text
        colors[ImGuiCol_TextSelectedBg]         = ImVec4{0.347f, 0.351f, 0.354f, 1.0f}; // Selected background
        
        /* Button */
        colors[ImGuiCol_Button]                 = ImVec4{0.195f, 0.203f, 0.206f, 1.0f}; // Normal
        colors[ImGuiCol_ButtonHovered]          = ImVec4{0.235f, 0.239f, 0.240f, 1.0f}; // Hover
        colors[ImGuiCol_ButtonActive]           = ImVec4{0.175f, 0.188f, 0.194f, 1.0f}; // Click
                
        /* Frame (checkbox, radio button, plot, slider, text input) */
        colors[ImGuiCol_FrameBg]                = ImVec4{0.195f, 0.203f, 0.206f, 1.0f}; // Normal
        colors[ImGuiCol_FrameBgHovered]         = ImVec4{0.235f, 0.239f, 0.240f, 1.0f}; // Hover
        colors[ImGuiCol_FrameBgActive]          = ImVec4{0.175f, 0.188f, 0.194f, 1.0f}; // Click
        
        /* Tab */
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4{0.175f, 0.188f, 0.194f, 1.0f}; // Normal background
        colors[ImGuiCol_TabUnfocused]           = ImVec4{0.175f, 0.188f, 0.194f, 1.0f}; // Normal background
        colors[ImGuiCol_Tab]                    = ImVec4{0.233f, 0.241f, 0.251f, 1.0f}; // Not selected, Panel Selected
        colors[ImGuiCol_TabActive]              = ImVec4{0.341f, 0.396f, 0.457f, 1.0f}; // Selected
        colors[ImGuiCol_TabHovered]             = ImVec4{0.447f, 0.524f, 0.611f, 1.0f}; // Hovered
         
        /* Pop Up */
        colors[ImGuiCol_PopupBg]                = ImVec4{0.166f, 0.172f, 0.177f, 1.0f}; // Background
        colors[ImGuiCol_Border]                 = ImVec4{0.297f, 0.297f, 0.297f, 1.0f}; // Border
        colors[ImGuiCol_BorderShadow]           = ImVec4{0.297f, 0.297f, 0.297f, 1.0f}; // Border
        
        /* Scrollbar */
        colors[ImGuiCol_ScrollbarBg]            = ImVec4{0.235f, 0.239f, 0.240f, 1.0f}; // Background
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4{0.102f, 0.105f, 0.109f, 1.0f}; // Scrollbar
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4{0.102f, 0.105f, 0.109f, 1.0f}; // Scrollbar
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4{0.102f, 0.105f, 0.109f, 1.0f}; // Scrollbar
        
        /* Separator */
        colors[ImGuiCol_Separator]              = ImVec4{0.347f, 0.351f, 0.354f, 1.0f};
        colors[ImGuiCol_SeparatorHovered]       = ImVec4{0.347f, 0.351f, 0.354f, 1.0f};
        colors[ImGuiCol_SeparatorActive]        = ImVec4{0.347f, 0.351f, 0.354f, 1.0f};
        
        /* Misc */
        //colors[ImGuiCol_CheckMark]              =
        //colors[ImGuiCol_SliderGrab]             =
        //colors[ImGuiCol_SliderGrabActive]       =
        //colors[ImGuiCol_ResizeGrip]             =
        //colors[ImGuiCol_ResizeGripHovered]      =
        //colors[ImGuiCol_ResizeGripActive]       =
        //colors[ImGuiCol_DockingPreview]         =
        //colors[ImGuiCol_PlotLines]              =
        //colors[ImGuiCol_PlotLinesHovered]       =
        //colors[ImGuiCol_PlotHistogram]          =
        //colors[ImGuiCol_PlotHistogramHovered]   =
        //colors[ImGuiCol_TableHeaderBg]          =
        //colors[ImGuiCol_TableBorderStrong]      =
        //colors[ImGuiCol_TableBorderLight]       =
        //colors[ImGuiCol_TableRowBg]             =
        //colors[ImGuiCol_TableRowBgAlt]          =
        //colors[ImGuiCol_DragDropTarget]         =
        //colors[ImGuiCol_NavHighlight]           =
    }
    
    void EditorLayer::LoadLayout()
    {
        std::string file =\
            FileSystem::GetRuntimeConfDirPath() + Parser::DEFAULTS::EDITOR_APP_LAYOUT_FILE;
        
        if ( LoadLayoutFile(file) )
        {
            LOG_TRACE("Loaded Editor Application layout from file {}", file);
        }
    }

    void EditorLayer::SetDefaultFontTTF(const std::string& pathToFileTTF, float size)
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.FontDefault = io.Fonts->AddFontFromFileTTF(pathToFileTTF.c_str(), size);
    }

    bool EditorLayer::LoadLayoutFile(const std::string& file)
    {
        s_layoutFile = file;
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.IniFilename = s_layoutFile.c_str();
        return true;
    }
    
}