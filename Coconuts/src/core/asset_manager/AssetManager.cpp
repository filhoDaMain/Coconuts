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

#include <coconuts/AssetManager.h>

namespace Coconuts
{
    
    /* Static Hash Tables Definitions */
    std::unordered_map<std::string, std::shared_ptr<Texture2D>>     AssetManager::m_HashTable_Textures2D;
    std::unordered_map<std::string, std::shared_ptr<Sprite>>        AssetManager::m_HashTable_Sprites;
    std::unordered_map<std::string, AssetManager::SpriteSelector>   AssetManager::m_HashTable_SpriteSlectors;
    
    /* Static Keys Lists Definitions */
    std::vector<std::string> AssetManager::m_KeysList_Textures2D;
    std::vector<std::string> AssetManager::m_KeysList_Sprites;
    
    
    
    //static
    bool AssetManager::ImportTexture2D(const std::string& logicalName, const std::string& path)
    {
        /* Create Texture2D from image path */
        std::shared_ptr<Texture2D> texture2D;
        texture2D.reset( Texture2D::Create(path) );
        
        /* Store */
        m_HashTable_Textures2D[logicalName] = texture2D;
        
        /* Update Keys List */
        m_KeysList_Textures2D.emplace_back(logicalName);
        
        return true;
    }
    
    //static
    std::shared_ptr<Texture2D> AssetManager::GetTexture2D(const std::string& logicalName)
    {
        auto found = m_HashTable_Textures2D.find(logicalName);
        
        if (found != m_HashTable_Textures2D.end())
        {
            return found->second;
        }
        
        return nullptr;
    }
    
    
    
    //static
    bool AssetManager::CreateSprite(const std::string& logicalName,
                                    const std::string& spriteSheetLogicalName,
                                    const SpriteSelector& selector)
    {
        /* Get Texture2D from spritesheet logical name */
        std::shared_ptr<Texture2D> texture2D = AssetManager::GetTexture2D(spriteSheetLogicalName);
        
        /* Create Sprite from spritesheet and SpriteSelector */
        std::shared_ptr<Sprite> sprite;
        sprite.reset( Sprite::Create(texture2D, selector.coords, selector.cellSize, selector.spriteSize) );
        
        /* Store Sprite */
        m_HashTable_Sprites[logicalName] = sprite;
        
        /* Store SpriteSelector */
        m_HashTable_SpriteSlectors[logicalName] = selector;
        
        /* Update Keys List */
        m_KeysList_Sprites.emplace_back(logicalName);
        
        return true;
    }
    
    //static
    std::shared_ptr<Sprite> AssetManager::GetSprite(const std::string& logicalName)
    {
        auto found = m_HashTable_Sprites.find(logicalName);
        
        if (found != m_HashTable_Sprites.end())
        {
            return found->second;
        }
        
        return nullptr;
    }
    
    //static
    std::tuple<bool, AssetManager::SpriteSelector> AssetManager::GetSpriteSelector(const std::string& logicalName)
    {
        auto found = m_HashTable_SpriteSlectors.find(logicalName);
        
        if (found != m_HashTable_SpriteSlectors.end())
        {
            return std::make_tuple(true, found->second);
        }
        
        return std::make_tuple(false, found->second);
    }
    
}