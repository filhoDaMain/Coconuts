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

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

namespace Coconuts
{
    
    namespace {
    namespace Parser
    {
        namespace DEFAULTS
        {
            constexpr auto EDITOR_APP_LAYOUT_FILE = "layout.ini";
        }
    }
    } //namespace
    
    
    void EditorLayer::OnUpdate(Timestep ts)
    {
        /**
         * SET UP DOCK SPACE
         * 
         * reference:
         *      imgui_demo.cpp:
         *          ShowExampleAppDockSpace() 
         */

        static bool dockspaceOpen = true;

        static bool opt_fullscreen_persistant = true;
        bool opt_fullscreen = opt_fullscreen_persistant;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->GetWorkPos());
            ImGui::SetNextWindowSize(viewport->GetWorkSize());
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
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
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));



        // DOCK SPACE BEGIN
        // ---------------------------------------------------------------------------
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

#if 0
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                ImGui::Separator();
                if (ImGui::MenuItem("Close DockSpace", NULL, false))
                    dockspaceOpen = false;
                ImGui::EndMenu();
            }


            ImGui::EndMenuBar();
        }
#endif
        
        /* PopUps triggering flags */
        static bool showPopUp_ImportTexture2D = false;
        static bool showPopUp_CreateSprite = false;
        
        
        /*  M E N U   B A R  */
        
        if (ImGui::BeginMenuBar())
        {
            /* File Menu */
            m_FileMenu.Draw();
            
            /* Entity Menu */
            m_EntityMenu.Draw();
            
            /* Assets Menu */
            m_AssetsMenu.Draw(&showPopUp_ImportTexture2D, &showPopUp_CreateSprite);
            
            ImGui::EndMenuBar();
        }
        
        
        
        /*  P O P   U P S  */
        
        if (showPopUp_ImportTexture2D)
        {
            /* Import Texture 2D (Triggered from AssetsMenu) */
            m_ImportTexture2dPopUp.Draw(&showPopUp_ImportTexture2D);
        }
        
        if (showPopUp_CreateSprite)
        {
            /* Create Sprite (Triggered from AssetsMenu) */
            m_CreateSpritePopUp.Draw(&showPopUp_CreateSprite);
        }
        
        if (m_ShowPopUp_LoadProject)
        {
            LOG_TRACE("Load Project");
            m_LoadProjectPopUp.Draw();
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

    void EditorLayer::OnPostAttach()
    {
        LOG_TRACE("Editor Layer OnPostAttach()");
        
        /* Init flags */
        m_ShowPopUp_LoadProject = false;
        
        /* Set Style */
        SetCustomGUIStyle();
        
        m_FileMenu.Init(m_GameLayerPtr, &m_ShowPopUp_LoadProject);
        m_EntityMenu.Init(m_GameLayerPtr);
        
        m_ViewportPanel.Init(m_GameLayerPtr);
        m_ComponentInspectorPanel.Init();
        m_SceneOverviewPanel.Init(m_GameLayerPtr, &m_ComponentInspectorPanel);
        m_AssetInspectorPanel.Init();
        m_AssetsPanel.Init(&m_AssetInspectorPanel);
        
        m_ImportTexture2dPopUp.Init();
        m_CreateSpritePopUp.Init();
        m_LoadProjectPopUp.Init(&m_ShowPopUp_LoadProject);
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
        
        if ( Editor::EditorApp::LoadLayoutFile(file) )
        {
            LOG_TRACE("Loaded Editor Application layout from file {}", file);
        }
    }
    
}