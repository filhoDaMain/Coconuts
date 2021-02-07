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

#include "AssetInspector.h"
#include <coconuts/editor.h>
#include <coconuts/AssetManager.h>
#include <coconuts/graphics/Texture.h>
#include <coconuts/Logger.h>
#include <tuple>
#include <string.h>

namespace Coconuts {
namespace Panels
{
 
    bool AssetInspector::Init()
    {
        DrawContextFunc = [&](void) { this->DrawEmpty(); };
        return true;
    }
    
    void AssetInspector::Draw()
    {
        ImGui::Begin("Asset Inspector");
        
        /* Draw current context */
        DrawContextFunc();
        
        ImGui::End();
    }
    
    void AssetInspector::DrawEmpty()
    {
        //such empty :)
    }
    
    void AssetInspector::DrawTexture2DAsset()
    {
        //Early debug
        
        auto texture = AssetManager::GetTexture2D(m_LogicalNameTexture2D);
        ImGui::Image((void *) *texture, ImVec2((texture->GetWidth()/3), (texture->GetHeight()/3)), ImVec2{0, 1}, ImVec2{1, 0});
    }
    
    void AssetInspector::DrawSpriteAsset()
    {
        ImGui::Text("Sprite");
        
        /* Sprite's name + Enable its edition */
        char spriteNameTextBox[32];
        memset(spriteNameTextBox, 0x00, sizeof(spriteNameTextBox));
        strcpy(spriteNameTextBox, m_LogicalNameSprite.c_str());
        ImGui::InputText(" ", spriteNameTextBox, sizeof(spriteNameTextBox));
        
        /* Get sprite's spritesheet texture image */
        auto sprite = AssetManager::GetSprite(m_LogicalNameSprite);
        auto texture = sprite->GetTexture();
        bool valid;
        AssetManager::SpriteSelector selector;
        std::tie(valid, selector) = AssetManager::GetSpriteSelector(m_LogicalNameSprite);
        
        /* Get Sprite's spritesheet name + Enable spritesheet switch from Drop-down menu */        
        auto spritesheetNames = AssetManager::GetAllTexture2DLogicalNames();
        const char* items[spritesheetNames.size()];
        
        /* Populate items[] and find spritesheet texture index */
        int selected_texture_index = 0;
        int i = 0;
        for (auto select : spritesheetNames)
        {
            items[i] = select.c_str();
            if (m_LogicalNameSprite.compare(select) == 0) selected_texture_index = i;
            i++;
        }
        
        /* Drop-down */
        ImGui::Text("Sprite sheet");
        ImGui::Combo("Texture", &selected_texture_index, items, IM_ARRAYSIZE(items));
        
        /* Display small sized texture */
        if (texture != nullptr)
        {
            ImGui::Image((void *) *texture, ImVec2((texture->GetWidth()/7), (texture->GetHeight()/7)), ImVec2{0, 1}, ImVec2{1, 0});
        }
        
        static AssetManager::SpriteSelector selectorEdit;
        static bool saved = true;
        if (saved)
        {
            selectorEdit = selector;
        }
        
        ImGui::Text("Coords X");
        ImGui::DragFloat("##X", &selectorEdit.coords.x, 0.1f);
        ImGui::Text("Coords Y");
        ImGui::DragFloat("##Y", &selectorEdit.coords.y, 0.1f);
        ImGui::Text("Cell Size X");
        ImGui::DragFloat("##A", &selectorEdit.cellSize.x, 0.1f);
        ImGui::Text("Cell Size Y");
        ImGui::DragFloat("##B", &selectorEdit.cellSize.y, 0.1f);
        ImGui::Text("Sprite Size X");
        ImGui::DragFloat("##H", &selectorEdit.spriteSize.x, 0.1f);
        ImGui::Text("Sprite Size Y");
        ImGui::DragFloat("##V", &selectorEdit.spriteSize.y, 0.1f);

        do
        {
            if (!valid)
            {
                break; //Impossible to display sprite. GOTO 'Save Button'.
            }

            /* Crop texture image to display only sprite's region */
            float uv0x = (float) (selectorEdit.coords.x * selectorEdit.cellSize.x) / texture->GetWidth();
            float uv0y = (float) (selectorEdit.coords.y * selectorEdit.cellSize.y + selectorEdit.cellSize.y) / texture->GetHeight();
            float uv1x = (float) (selectorEdit.coords.x * selectorEdit.cellSize.x + selectorEdit.cellSize.x) / texture->GetWidth();
            float uv1y = (float) (selectorEdit.coords.y * selectorEdit.cellSize.y) / texture->GetHeight();
            ImVec2 uv0 = ImVec2(uv0x, uv0y);
            ImVec2 uv1 = ImVec2(uv1x, uv1y);

            /* Display sprite */
            ImGui::Image((void *) *texture, ImVec2(selectorEdit.cellSize.x/2, selectorEdit.cellSize.y/2), uv0, uv1);
            
        } while(false);
        
        /* Save Edits ? */
        saved = false;
        if (ImGui::Button("Save"))
        {
            /* Create New Sprite */
            
        }
        
    }
    
    
    /* Change Context - Texture2D */
    void AssetInspector::ChangeContext2Texture2D(std::string& name)
    {
        DrawContextFunc = [&](void) { this->DrawTexture2DAsset(); };
        m_LogicalNameTexture2D = name;
    }
    
    /* Change Context - Sprite */
    void AssetInspector::ChangeContext2Sprite(std::string& name)
    {
        DrawContextFunc = [&](void) { this->DrawSpriteAsset(); };
        m_LogicalNameSprite = name;
    }
    
}
}