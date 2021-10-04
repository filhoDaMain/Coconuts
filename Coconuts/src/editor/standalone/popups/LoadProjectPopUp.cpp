/*
 * Copyright 2021 Andre Temprilho
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

#include "LoadProjectPopUp.h"
#include <coconuts/Application.h>
#include <coconuts/editor.h>
#include <coconuts/Logger.h>
#include <nfd/nfd.h>
#include "../ed_utils.h"

namespace Coconuts {
namespace PopUps
{
    
    bool LoadProjectPopUp::Init(GameLayer*& gameLayer, Panels::ComponentInspector* cmpInsp, bool* m_ShowPopUpLoadProj)
    {
        m_GameLayerPtr = gameLayer;
        m_ComponentInspectorPtr = cmpInsp;
        m_ShowPopUp = m_ShowPopUpLoadProj;
        return true;
    }
    
    void LoadProjectPopUp::Draw()
    {
        ImGui::OpenPopup("Application:  Load Project File");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Application:  Load Project File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {            
            static char pathBuffer[200] = "/absolute/path/to/project.ccnproj";
            
            /* Image Path */
            ImGui::Text("Load ccnproj file");
            ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
            if (ImGui::InputText("", pathBuffer, sizeof(pathBuffer)))
            {
                //
            }
            
            ImGui::SameLine();
            if (ImGui::Button("Open        "))
            {
                /* Open File Dialog */
                nfdchar_t *outPath = NULL;
                
                /* WARNING -> outPath is dynamically allocated and needs to be freed after */
                nfdresult_t result = NFD_OpenDialog( "ccnproj", NULL, &outPath );
    
                if ( result == NFD_OKAY )
                {
                    /* Copy to static buffer and free dynamically allocated outPath */
                    strcpy(pathBuffer, outPath);
                    free(outPath);  // NEEDED!
                }
                
                else if ( result == NFD_CANCEL )
                {
                    //do nothing
                }
                else 
                {
                    LOG_WARN("Failed to load file from File Dialog");
                }
            }
            
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            
            /* Import into Application */
            if (ImGui::Button("Import", ImVec2(120, 0)))
            {
                if ( strcmp(pathBuffer, "/absolute/path/to/project.ccnproj") != 0 )
                {
                    /* Clear context */
                    m_ComponentInspectorPtr->Init();
                    
                    /* Load project from .ccnproj file */
                    std::string path = std::string(pathBuffer);
                    AppManagerProxy::LoadRuntimeConfig(path);
                    
                    /* Store .ccnproj file for future reference when using 'Save' only */
                    utils::SaveState::StoreCCNProjFilePath(path);
                    
                    /**
                     * Workaround:
                     *  When loading a project from Editor GUI, set current active scene
                     *  main camera halt state to true, so it matches the default
                     *  Viewport panel focus state.
                     */
                    m_GameLayerPtr->HaltEvents(true); // override halt parameter from config file
                    
                    strcpy(pathBuffer, "/absolute/path/to/project.ccnproj");
                    
                    ImGui::CloseCurrentPopup();
                    *m_ShowPopUp = false;
                }
                else
                {
                    LOG_WARN("Path to file not defined yet!");
                }
            }
            ImGui::SetItemDefaultFocus();
            
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                /* Clear buffers for next usage */
                strcpy(pathBuffer, "/absolute/path/to/project.ccnproj");
                    
                ImGui::CloseCurrentPopup();
                *m_ShowPopUp = false;
            }
            
            ImGui::EndPopup();
        }
    }
    
}
}