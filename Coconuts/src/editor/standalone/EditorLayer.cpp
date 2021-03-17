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
#include <cstdint>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

namespace Coconuts
{
    
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
        
        
        /*  M E N U   B A R  */
        
        if (ImGui::BeginMainMenuBar())
        {
            /* File Menu */
            m_FileMenu.Draw();
            
            /* Entity Menu */
            m_EntityMenu.Draw();
            
            /* Assets Menu */
            m_AssetsMenu.Draw(&showPopUp_ImportTexture2D);
            
            ImGui::EndMainMenuBar();
        }
        
        
        
        /*  P O P   U P S  */
        
        if (showPopUp_ImportTexture2D)
        {
            /* Import Texture 2D (Triggered from AssetsMenu) */
            m_ImportTexture2dPopUp.Draw(&showPopUp_ImportTexture2D);
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
        
        SetDarkThemeColors();
        m_FileMenu.Init(m_GameLayerPtr);
        m_EntityMenu.Init(m_GameLayerPtr);
        
        m_ViewportPanel.Init(m_GameLayerPtr);
        m_ComponentInspectorPanel.Init();
        m_SceneOverviewPanel.Init(m_GameLayerPtr, &m_ComponentInspectorPanel);
        m_AssetInspectorPanel.Init();
        m_AssetsPanel.Init(&m_AssetInspectorPanel);
        
        m_ImportTexture2dPopUp.Init();
    }
    
    void EditorLayer::SetDarkThemeColors()
    {
        LOG_TRACE("Using Dark Theme for Editor GUI");
        
        //TODO
#if 0
        ImVec4* colors = ImGui::GetStyle().Colors;
        
        /* Window */
        colors[ImGuiCol_WindowBg]               = ImVec4{r, g, b, a};
        colors[ImGuiCol_ChildBg]                =
        colors[ImGuiCol_NavWindowingHighlight]  = 
        colors[ImGuiCol_NavWindowingDimBg]      =
        colors[ImGuiCol_ModalWindowDimBg]       =
        
        /* Menu Bar */
        colors[ImGuiCol_MenuBarBg]              =
                
        /* Header */
        colors[ImGuiCol_Header]                 =
        colors[ImGuiCol_HeaderHovered]          =
        colors[ImGuiCol_HeaderActive]           =
        
        /* Title */
        colors[ImGuiCol_TitleBg]                =
        colors[ImGuiCol_TitleBgActive]          =
        colors[ImGuiCol_TitleBgCollapsed]       =
                
        /* Text */        
        colors[ImGuiCol_Text]                   =
        colors[ImGuiCol_TextDisabled]           =       
        colors[ImGuiCol_TextSelectedBg]         =
        
        /* Button */
        colors[ImGuiCol_Button]                 =
        colors[ImGuiCol_ButtonHovered]          =
        colors[ImGuiCol_ButtonActive]           =
                
        /* Frame (checkbox, radio button, plot, slider, text input) */
        colors[ImGuiCol_FrameBg]                =
        colors[ImGuiCol_FrameBgHovered]         =
        colors[ImGuiCol_FrameBgActive]          =
        
        /* Tab */
        colors[ImGuiCol_Tab]                    =
        colors[ImGuiCol_TabHovered]             =
        colors[ImGuiCol_TabActive]              =
        colors[ImGuiCol_TabUnfocused]           =
        colors[ImGuiCol_TabUnfocusedActive]     =
        
        /* Misc */
        colors[ImGuiCol_PopupBg]                =
        colors[ImGuiCol_Border]                 =
        colors[ImGuiCol_BorderShadow]           =
        colors[ImGuiCol_ScrollbarBg]            =
        colors[ImGuiCol_ScrollbarGrab]          =
        colors[ImGuiCol_ScrollbarGrabHovered]   =
        colors[ImGuiCol_ScrollbarGrabActive]    =
        colors[ImGuiCol_CheckMark]              =
        colors[ImGuiCol_SliderGrab]             =
        colors[ImGuiCol_SliderGrabActive]       =
        colors[ImGuiCol_Separator]              =
        colors[ImGuiCol_SeparatorHovered]       =
        colors[ImGuiCol_SeparatorActive]        =
        colors[ImGuiCol_ResizeGrip]             =
        colors[ImGuiCol_ResizeGripHovered]      =
        colors[ImGuiCol_ResizeGripActive]       =
        colors[ImGuiCol_DockingPreview]         =
        colors[ImGuiCol_DockingEmptyBg]         =
        colors[ImGuiCol_PlotLines]              =
        colors[ImGuiCol_PlotLinesHovered]       =
        colors[ImGuiCol_PlotHistogram]          =
        colors[ImGuiCol_PlotHistogramHovered]   =
        colors[ImGuiCol_TableHeaderBg]          =
        colors[ImGuiCol_TableBorderStrong]      =
        colors[ImGuiCol_TableBorderLight]       =
        colors[ImGuiCol_TableRowBg]             =
        colors[ImGuiCol_TableRowBgAlt]          =
        colors[ImGuiCol_DragDropTarget]         =
        colors[ImGuiCol_NavHighlight]           =
#endif
    }
    
}