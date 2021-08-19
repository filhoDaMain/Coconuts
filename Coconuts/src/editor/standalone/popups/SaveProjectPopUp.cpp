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

#include "SaveProjectPopUp.h"
#include <coconuts/editor.h>
#include <coconuts/AssetManager.h>
#include <coconuts/ecs/Serializer.h>
#include <string>
#include <coconuts/Logger.h>
#include <iostream>
#include <fstream>  
#include <nfd/nfd.h>

namespace Coconuts {
    
namespace
{
    const std::string FILE_HEADER =\
R"(# This is a Coconuts project configuration file.
#
# Coconuts is an Open Source Game Engine project Licensed under the
#  Apache License, Version 2.0 (Copyright 2021).
#
# Please refer to https://github.com/filhoDaMain/Coconuts for up-to-date
#  information about Licensing, Copyright, Authorship and Acknowledgements.
#
)";
}
    
namespace PopUps
{
    
    static bool SaveCCNProjFile(const std::string& filePath)
    {
        Serializer serializer;
        std::ofstream outfile (filePath);
        outfile << FILE_HEADER << std::endl;
        outfile << AssetManager::Serialize() << std::endl;
        outfile << std::endl;
        outfile << serializer.Serialize() << std::endl;
        outfile.close();
           
        return true;
    }
    
    bool SaveProjectPopUp::Init(bool* showPopUpSaveProj)
    {
        m_ShowPopUp = showPopUpSaveProj;
        return true;
    }
    
    void SaveProjectPopUp::Draw()
    {
        ImGui::OpenPopup("Application:  Save Project File");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("Application:  Save Project File", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {            
            static char pathBuffer[200] = "/absolute/path/to/project.ccnproj";
            
            /* Image Path */
            ImGui::Text("Save ccnproj file");
            ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
            
            ImGui::Text("File:");
            ImGui::Text("%s", pathBuffer);
            
            ImGui::Spacing(); ImGui::Spacing();
            if (ImGui::Button("Open File Dialog        "))
            {
                /* Open File Dialog */
                nfdchar_t *outPath = NULL;
                
                /* WARNING -> outPath is dynamically allocated and needs to be freed after */
                nfdresult_t result = NFD_SaveDialog( "ccnproj", NULL, &outPath );
    
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
            if (ImGui::Button("Save", ImVec2(120, 0)))
            {
                if ( strcmp(pathBuffer, "/absolute/path/to/project.ccnproj") != 0 )
                {
                    std::string path = std::string(pathBuffer);
                    SaveCCNProjFile(path);
                    
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