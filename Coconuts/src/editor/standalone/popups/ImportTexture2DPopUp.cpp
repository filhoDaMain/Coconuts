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

#include "ImportTexture2DPopUp.h"
#include <coconuts/editor.h>
#include <coconuts/AssetManager.h>
#include <coconuts/Logger.h>
#include <string>

namespace Coconuts {
namespace PopUps
{
    
    void ImportTexture2DPopUp::Draw(bool* show)
    {   
        ImGui::OpenPopup("AssetManager:  Import Texture 2D");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("AssetManager:  Import Texture 2D", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {            
            static char pathBuffer[200] = "/absolute/path/to/image.png";
            static char logicalNameBuffer[32];
            
            /* Image Path */
            ImGui::Text("Select an image to import from filesystem");
            ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
            if (ImGui::InputText("ABS PATH", pathBuffer, sizeof(pathBuffer)))
            {
                //
            }
            
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            
            /* Image Path */
            ImGui::Text("Give it a Logical Name to reference from code");
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            if (ImGui::InputText("LOGICAL NAME", logicalNameBuffer, sizeof(logicalNameBuffer)))
            {
                //
            }
            
            ImGui::Separator();


            if (ImGui::Button("Import", ImVec2(120, 0)))
            {
                std::string path = std::string(pathBuffer);
                std::string name = std::string(logicalNameBuffer);
                AssetManager::ImportTexture2D(name, path);
                
                ImGui::CloseCurrentPopup();
                *show = false;
            }
            ImGui::SetItemDefaultFocus();
            
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                ImGui::CloseCurrentPopup();
                *show = false;
            }
            
            ImGui::EndPopup();
        }
        
    }
    
}
}