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
#include <nfd/nfd.h>
#include <string>
#include <cstring>

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
            static char logicalNameBuffer[32] = "SingleWordName";
            
            /* Image Path */
            ImGui::Text("Select an image from filesystem");
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
                nfdresult_t result = NFD_OpenDialog( "png,jpg", NULL, &outPath );
    
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
            
            /* Image Path */
            ImGui::Text("Give it a Logical Name to reference from code");
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            if (ImGui::InputText("Logical Name", logicalNameBuffer, sizeof(logicalNameBuffer)))
            {
                //
            }
            
            ImGui::Separator();

            
            /* Import into AssetManager */
            if (ImGui::Button("Import", ImVec2(120, 0)))
            {
                if ( (strcmp(pathBuffer, "/absolute/path/to/image.png") != 0) && 
                     (strcmp(logicalNameBuffer, "SingleWordName") != 0) )
                {
                    std::string path = std::string(pathBuffer);
                    std::string name = std::string(logicalNameBuffer);
                    
                    if ( AssetManager::ImportTexture2D(name, path) )
                    {
                        LOG_TRACE("Imported file {} into AssetManager as '{}' Texture2D", path, name);
                    }
                    else
                    {
                        LOG_WARN("Failed to import. File {} does not exist!", path);
                    }
                    
                    /* Clear buffers for next usage */
                    strcpy(pathBuffer, "/absolute/path/to/image.png");
                    strcpy(logicalNameBuffer, "SingleWordName");
                    
                    ImGui::CloseCurrentPopup();
                    *show = false;
                }
                else
                {
                    LOG_WARN("Path to file or Logical Name not defined yet!");
                }
            }
            ImGui::SetItemDefaultFocus();
            
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                /* Clear buffers for next usage */
                strcpy(pathBuffer, "/absolute/path/to/image.png");
                strcpy(logicalNameBuffer, "SingleWordName");
                    
                ImGui::CloseCurrentPopup();
                *show = false;
            }
            
            ImGui::EndPopup();
        }
        
    }
    
}
}