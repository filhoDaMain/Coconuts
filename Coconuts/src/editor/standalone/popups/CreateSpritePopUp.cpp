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

#include "CreateSpritePopUp.h"
#include <coconuts/editor.h>
#include <coconuts/AssetManager.h>
#include <coconuts/Logger.h>
#include <coconuts/graphics/Texture.h>
#include <glm/glm.hpp>
#include "../ed_utils.h"
#include <string.h>
#include <sstream>

namespace Coconuts {
namespace PopUps
{
    
    void CreateSpritePopUp::Draw(bool* show)
    {
        ImGui::OpenPopup("AssetManager:  Create New Sprite");

        // Always center this window when appearing
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal("AssetManager:  Create New Sprite", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char logicalNameBuffer[32] = "SingleWordName";
            
            auto sheets = AssetManager::GetAllTexture2DLogicalNames();
            std::vector<char*> sheetsArray;
            sheetsArray.reserve(sheets.size());

            static int seletected_sheet_index = 0;
            int i;
            for (i = 0; i < sheets.size(); i++)
            {
                sheetsArray.push_back(const_cast<char*>(sheets[i].c_str())); 
            }

            /* Drop-down - Choose spritesheet */
            ImGui::Text("Create sprite from sprite sheet Texture2D asset");
            ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
            ImGui::Combo("Texture2D", &seletected_sheet_index, &sheetsArray[0], sheetsArray.size());
            
            /* Draw sprite sheet preview */
            std::string texture2DStr = sheetsArray[seletected_sheet_index];
            auto texture = AssetManager::GetTexture2D(texture2DStr);
            ImGui::Image((void *) *texture, ImVec2((texture->GetWidth()/8), (texture->GetHeight()/8)), ImVec2{0, 1}, ImVec2{1, 0});
            
            
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            
            ImGui::Text("Give Sprite a logical name");
            ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();
            if (ImGui::InputText("Sprite Name", logicalNameBuffer, sizeof(logicalNameBuffer)))
            {
                //
            }
            
            ImGui::Separator();
            
            /* Create Sprite in AssetManager */
            if (ImGui::Button("Create", ImVec2(120, 0)))
            {
                if ((strcmp(logicalNameBuffer, "SingleWordName") != 0))
                {
                    //texture2DStr is Texture2D name as string
                    std::string spriteNameStr = std::string(logicalNameBuffer);
                    
                    /* Using a default selector that takes the whole sprite sheet */
                    AssetManager::SpriteSelector selector;
                    selector.cellSize = glm::vec2(1.0f);
                    selector.coords = glm::vec2(1.0f);
                    selector.spriteSize = glm::vec2(1.0f);
                    AssetManager::CreateSprite(spriteNameStr, texture2DStr, selector);
                    
                    /* Clear buffer for next usage */
                    strcpy(logicalNameBuffer, "SingleWordName");
                    
                    ImGui::CloseCurrentPopup();
                    *show = false;
                }
                else
                {
                    LOG_WARN("Sprite Name not defined yet!");
                }
            }
            
            ImGui::SetItemDefaultFocus();
            
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0)))
            {
                /* Clear buffers for next usage */
                strcpy(logicalNameBuffer, "SingleWordName");
                
                ImGui::CloseCurrentPopup();
                *show = false;
            }
            
            ImGui::EndPopup();
        }
    }
    
}
}