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

#include "Assets.h"
#include <coconuts/editor.h>
#include <coconuts/AssetManager.h>
#include <string>
#include <vector>
#include <coconuts/Logger.h>

namespace Coconuts {
namespace Panels
{
    
    bool Assets::Init(AssetInspector* assetInspector)
    {
        m_AssetInspectorPtr = assetInspector;
        return true;
    }
    
    void Assets::Draw()
    {
        ImGui::Begin("Assets");
        ImGui::Spacing();ImGui::Spacing();
        
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Asset Manager - Logical References:");
        ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
        
        DrawTextures2DTree();
        
        ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();ImGui::Spacing();ImGui::Spacing();
        
        DrawSpritesTree();
        
        ImGui::End();
    }
    
    /* Textures2D */
    void Assets::DrawTextures2DTree()
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        
        if (ImGui::TreeNodeEx("Textures 2D" , flags))
        {
            /* Display all logical names */
            for (auto name : AssetManager::GetAllTexture2DLogicalNames())
            {
                static std::string context_str = "";
                
                ImGuiTreeNodeFlags inner_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Bullet;
                inner_flags |=  context_str.compare(name) == 0 ? ImGuiTreeNodeFlags_Selected : 0;   
                 
                bool fakeopen = ImGui::TreeNodeEx(name.c_str() , inner_flags);
                if (ImGui::IsItemClicked())
                {
                    static bool toggleSelected = false;
                    toggleSelected = !toggleSelected;
                    context_str = toggleSelected ? name : "";
                }
                if (fakeopen) ImGui::TreePop();

                /**
                 * This Texture2D Logical Name is currently selected.
                 */
                if (inner_flags & ImGuiTreeNodeFlags_Selected)
                {
                    m_AssetInspectorPtr->ChangeContext2Texture2D(name);
                }
            }
            
            ImGui::TreePop();
        }
    }
    
    /* Sprites */
    void Assets::DrawSpritesTree()
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
        
        if (ImGui::TreeNodeEx("Sprites" , flags))
        {
            /* Display all logical names */
            for (auto name : AssetManager::GetAllSpriteLogicalNames())
            {
                static std::string context_str = "";
                
                ImGuiTreeNodeFlags inner_flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Bullet;
                inner_flags |=  context_str.compare(name) == 0 ? ImGuiTreeNodeFlags_Selected : 0;   
                 
                bool fakeopen = ImGui::TreeNodeEx(name.c_str() , inner_flags);
                if (ImGui::IsItemClicked())
                {
                    static bool toggleSelected = false;
                    toggleSelected = !toggleSelected;
                    context_str = toggleSelected ? name : "";
                }
                if (fakeopen) ImGui::TreePop();

                /**
                 * This Sprite Logical Name is currently selected.
                 */
                if (inner_flags & ImGuiTreeNodeFlags_Selected)
                {
                    //change component inspector context to display its data
                }
            }
            
            ImGui::TreePop();
        }
    }
    
}
}