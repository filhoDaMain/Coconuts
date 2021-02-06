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
        ImGui::Image((void *) *texture, ImVec2((texture->GetWidth()/6), (texture->GetHeight()/6)), ImVec2{0, 1}, ImVec2{1, 0});
    }
    
    void AssetInspector::DrawSpriteAsset()
    {
        auto sprite = AssetManager::GetSprite(m_LogicalNameSprite);
        auto texture = sprite->GetTexture();
        bool valid;
        AssetManager::SpriteSelector selector;
        std::tie(valid, selector) = AssetManager::GetSpriteSelector(m_LogicalNameSprite);
        
        if (!valid)
        {
            return; //impossible to coninue
        }
        
        /* Crop texture image to display only sprite's region */
        float uv0x = (float) (selector.coords.x) / texture->GetWidth();
        float uv0y = (float) (selector.coords.y + selector.cellSize.y) / texture->GetHeight();
        float uv1x = (float) (selector.coords.x + selector.cellSize.x) / texture->GetWidth();
        float uv1y = (float) (selector.coords.y) / texture->GetHeight();
        ImVec2 uv0 = ImVec2(uv0x, uv0y);
        ImVec2 uv1 = ImVec2(uv1x, uv1y);
        
        ImGui::Image((void *) *texture, ImVec2(selector.cellSize.x/2, selector.cellSize.y/2), uv0, uv1);
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