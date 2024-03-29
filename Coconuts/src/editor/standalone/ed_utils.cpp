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

#include "ed_utils.h"
#include <coconuts/editor.h>
#include <iostream>
#include <fstream> 

namespace Coconuts
{
    
    void utils::DrawTableVec2(const std::string& label, glm::vec2& values, float colWidth)
    {
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);
        
        // Col 0
        ImGui::Text("%s", label.c_str());
        
        // Col 1
        ImGui::NextColumn();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 4});
        
        if (ImGui::Button("X"))
        {
            values.x = 0.0f;
        }    
        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f);

        if (ImGui::Button("Y"))
        {
            values.y = 0.0f;
        }    
        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f);
        
        ImGui::PopStyleVar(1);
        ImGui::Columns(1);
        
        ImGui::PopID();
        ImGui::Spacing(); ImGui::Spacing(); 
    }
    
    void utils::DrawTableFloat(const std::string& label, const std::string& param, float& values, bool positive, float colWidth)
    {
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);
        
        // Col 0
        ImGui::Text("%s", label.c_str());
        
        // Col 1
        ImGui::NextColumn();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 4});
        
        if (ImGui::Button(param.c_str()))
        {
            values = 0.0f;
        }    
        ImGui::SameLine();
        if (positive)
        {
            ImGui::DragFloat("##X", &values, 0.1f, 0, 100);
        }
        else
        {
            ImGui::DragFloat("##X", &values, 0.1f);
        }
        
        ImGui::PopStyleVar(1);
        ImGui::Columns(1);
        
        ImGui::PopID();
        ImGui::Spacing(); ImGui::Spacing();
    }
    
    void utils::DrawTableTextButton(const std::string& label, const std::string& text, float colWidth)
    {
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);
        
        // Col 0
        ImGui::Text("%s", label.c_str());
        
        // Col 1
        ImGui::NextColumn();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 4});
        
        if (ImGui::Button(text.c_str()))
        {
            
        }
        
        ImGui::PopStyleVar(1);
        ImGui::Columns(1);
        
        ImGui::PopID();
        ImGui::Spacing(); ImGui::Spacing();
    }
    
    void utils::DrawTableImage(const std::string& label,
                               Texture2D& texture,
                               const glm::vec4& tintColor,
                               uint8_t divider,
                               float colWidth)
    {
        ImVec4 tint = ImVec4(tintColor.r, tintColor.g, tintColor.b, tintColor.a);
        
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);
        
        // Col 0
        ImGui::Text("%s", label.c_str());
        
        // Col 1
        ImGui::NextColumn();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 4});
        
        ImGui::Image((void *) texture, ImVec2((texture.GetWidth()/divider), (texture.GetHeight()/divider)), ImVec2{0, 1}, ImVec2{1, 0}, tint);
        
        ImGui::PopStyleVar(1);
        ImGui::Columns(1);
        
        ImGui::PopID();
        ImGui::Spacing(); ImGui::Spacing();
    }
    
    void utils::DrawTableImage(const std::string& label,
                               Texture2D& spriteSheet,
                               AssetManager::SpriteSelector& selector,
                               const glm::vec4& tintColor,
                               uint8_t divider,
                               float colWidth)
    {
        /* Crop texture image to display only sprite's region */
        float uv0x = (float) (selector.coords.x * selector.cellSize.x) / spriteSheet.GetWidth();
        float uv0y = (float) (selector.coords.y * selector.cellSize.y + selector.cellSize.y * selector.spriteSize.y) / spriteSheet.GetHeight();
        float uv1x = (float) (selector.coords.x * selector.cellSize.x + selector.cellSize.x * selector.spriteSize.x) / spriteSheet.GetWidth();
        float uv1y = (float) (selector.coords.y * selector.cellSize.y) / spriteSheet.GetHeight();
        ImVec2 uv0 = ImVec2(uv0x, uv0y);
        ImVec2 uv1 = ImVec2(uv1x, uv1y);
        ImVec4 tint = ImVec4(tintColor.r, tintColor.g, tintColor.b, tintColor.a);
        
        //TODO - Manage to re-use next lines from previous function
        
        ImGui::PushID(label.c_str());
        
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colWidth);
        
        // Col 0
        ImGui::Text("%s", label.c_str());
        
        // Col 1
        ImGui::NextColumn();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2, 4});
        
        ImGui::Image((void *) spriteSheet, ImVec2(selector.cellSize.x/divider, selector.cellSize.y/divider), uv0, uv1, tint);
        
        ImGui::PopStyleVar(1);
        ImGui::Columns(1);
        
        ImGui::PopID();
        ImGui::Spacing(); ImGui::Spacing();
    }
    
    
    /* SaveState */
    std::string utils::SaveState::s_FilePath("nullptr");    //static allocation
    
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
    
    //static
    bool utils::SaveState::SaveCCNProjFile(const std::string& filePath)
    {
        /* Store for future reference when using 'Save' only */
        s_FilePath = filePath;
        
        Serializer serializer;
        std::ofstream outfile (filePath);
        outfile << FILE_HEADER << std::endl;
        outfile << AssetManager::Serialize() << std::endl;
        outfile << std::endl;
        outfile << serializer.Serialize() << std::endl;
        outfile.close();
           
        return true;
    }
    
}