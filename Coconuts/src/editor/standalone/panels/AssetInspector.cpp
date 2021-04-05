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
        spriteSaved = true;
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
        std::string origSpriteSheetName;
        
        static char editSpriteName[32];
        if (spriteSaved)
        {
            /* Begin with original sprite name */
            strcpy(editSpriteName, m_LogicalNameSprite.c_str());
        }
        
        /* Edit Sprite name */
        ImGui::Text("Sprite Name");
        if (ImGui::InputText(" ", editSpriteName, sizeof(editSpriteName)))
        {
            //do nothing
        }
        
        
        /* Get sprite sheet name */
        bool found;
        std::tie(found, origSpriteSheetName) = AssetManager::GetSpriteSheetName(m_LogicalNameSprite);
        
        auto sheets = AssetManager::GetAllTexture2DLogicalNames();
        std::vector<char*> sheetsArray;
        sheetsArray.reserve(sheets.size());

        static int seletected_sheet_index = 0;
        int i;
        for (i = 0; i < sheets.size(); i++)
        {
            sheetsArray.push_back(const_cast<char*>(sheets[i].c_str()));
            
            if (spriteSaved && origSpriteSheetName.compare(sheetsArray[i]) == 0)
            {
                seletected_sheet_index = i; // begin with original sprite sheet
            }   
        }
        
        /* Drop-down - Choose spritesheet */
        ImGui::Text("Sprite sheet");
        ImGui::Combo("Texture", &seletected_sheet_index, &sheetsArray[0], sheetsArray.size());
        
        /* Draw spritesheet texture preview */
        auto texture = AssetManager::GetTexture2D(sheets[seletected_sheet_index]);
        ImGui::Image((void *) *texture, ImVec2((texture->GetWidth()/6), (texture->GetHeight()/6)), ImVec2{0, 1}, ImVec2{1, 0});
        
        /* Get SpriteSelector and enable changes */
        static AssetManager::SpriteSelector selectorEdit;
        bool valid;
        if (spriteSaved)
        {
            std::tie(valid, selectorEdit) = AssetManager::GetSpriteSelector(m_LogicalNameSprite);
            if (!valid)
            {
                return;
            }
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
        
        /* Crop texture image to display only sprite's region */
        float uv0x = (float) (selectorEdit.coords.x * selectorEdit.cellSize.x) / texture->GetWidth();
        float uv0y = (float) (selectorEdit.coords.y * selectorEdit.cellSize.y + selectorEdit.cellSize.y * selectorEdit.spriteSize.y) / texture->GetHeight();
        float uv1x = (float) (selectorEdit.coords.x * selectorEdit.cellSize.x + selectorEdit.cellSize.x * selectorEdit.spriteSize.x) / texture->GetWidth();
        float uv1y = (float) (selectorEdit.coords.y * selectorEdit.cellSize.y) / texture->GetHeight();
        ImVec2 uv0 = ImVec2(uv0x, uv0y);
        ImVec2 uv1 = ImVec2(uv1x, uv1y);
        
        /* Display sprite */
        ImGui::Image((void *) *texture, ImVec2(selectorEdit.cellSize.x/2, selectorEdit.cellSize.y/2), uv0, uv1);
        
        /* Save */
        spriteSaved = false;
        if (ImGui::Button("Save"))
        {
            /* Back to string */
            std::string editSpriteName2String = editSpriteName;
            
            /* If sprite name changed, delete old and create new */
            if (editSpriteName2String.compare(m_LogicalNameSprite) != 0)
            {
                AssetManager::DeleteSprite(m_LogicalNameSprite);
                AssetManager::CreateSprite(editSpriteName2String, sheets[seletected_sheet_index], selectorEdit);
                m_LogicalNameSprite = editSpriteName2String;
            }
            else
            {
                AssetManager::UpdateSprite(m_LogicalNameSprite, sheets[seletected_sheet_index], selectorEdit);
            }
            
            spriteSaved = true;
        }
    }
    
    
    /* Change Context - Texture2D */
    void AssetInspector::ChangeContext2Texture2D(std::string& name)
    {
        LOG_TRACE("AssetInspector - Change context to Texture2D");
        DrawContextFunc = [&](void) { this->DrawTexture2DAsset(); };
        m_LogicalNameTexture2D = name;
    }
    
    /* Change Context - Sprite */
    void AssetInspector::ChangeContext2Sprite(std::string& name)
    {
        LOG_TRACE("AssetInspector - Change context to Sprite");
        DrawContextFunc = [&](void) { this->DrawSpriteAsset(); };
        m_LogicalNameSprite = name;
        spriteSaved = true; // fetch original sprite data
    }
    
}
}