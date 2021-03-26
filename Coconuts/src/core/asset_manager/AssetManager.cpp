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
#include <coconuts/Logger.h>

namespace Coconuts
{
    
    /* Static Hash Tables Definitions */
    std::unordered_map<std::string, AssetManager::IndexedTexture2D> AssetManager::m_HashTable_Textures2D;
    std::unordered_map<std::string, AssetManager::IndexedSprite>    AssetManager::m_HashTable_Sprites;
    std::unordered_map<std::string, AssetManager::SpriteSelector>   AssetManager::m_HashTable_SpriteSelectors;
    
    /* Static Keys Lists Definitions */
    std::vector<std::string> AssetManager::m_KeysList_Textures2D;
    std::vector<std::string> AssetManager::m_KeysList_Sprites;
    
    
    
    //static (non mandatory)
    void AssetManager::Init()
    {
        m_HashTable_Textures2D.reserve(HashTableDefs::Textures2DHT::reserve);
        m_HashTable_Textures2D.max_load_factor(HashTableDefs::Textures2DHT::max_load_factor);
        
        m_HashTable_Sprites.reserve(HashTableDefs::SpritesHT::reserve);
        m_HashTable_Sprites.max_load_factor(HashTableDefs::SpritesHT::max_load_factor);
        m_HashTable_SpriteSelectors.reserve(HashTableDefs::SpritesHT::reserve);
        m_HashTable_SpriteSelectors.max_load_factor(HashTableDefs::SpritesHT::max_load_factor);
    }
    
    
    
    //static
    bool AssetManager::ImportTexture2D(const std::string& logicalName, const std::string& path)
    {
#if LOG_PROFILE_HASHTABLES
        LOG_TRACE("Pre-call ImportTexture2D():");
        LOG_TRACE("  m_HashTable_Textures2D.size    = {}", m_HashTable_Textures2D.size());
        LOG_TRACE("  m_HashTable_Textures2D.buckets = {}", m_HashTable_Textures2D.bucket_count());
        LOG_TRACE("  m_HashTable_Textures2D.load    = {}", m_HashTable_Textures2D.load_factor());
        LOG_TRACE("  m_HashTable_Textures2D.maxload = {}", m_HashTable_Textures2D.max_load_factor());
#endif
        
        /* Create Texture2D from image path */
        Texture2D* rawPtr = Texture2D::Create(path);
        if (rawPtr == nullptr)
        {
            return false;   // Failed. Stop here.
        }
         
        std::shared_ptr<Texture2D> texture2D;
        texture2D.reset(rawPtr);
        
        /* Store */
        IndexedTexture2D indexed = { texture2D, static_cast<uint32_t>(m_KeysList_Textures2D.size()) };
        m_HashTable_Textures2D[logicalName] = indexed;
        
        /* Update Keys List */
        m_KeysList_Textures2D.emplace_back(logicalName);
        
#if LOG_PROFILE_HASHTABLES
        LOG_TRACE("Post-call ImportTexture2D():");
        LOG_TRACE("  m_HashTable_Textures2D.size    = {}", m_HashTable_Textures2D.size());
        LOG_TRACE("  m_HashTable_Textures2D.buckets = {}", m_HashTable_Textures2D.bucket_count());
        LOG_TRACE("  m_HashTable_Textures2D.load    = {}", m_HashTable_Textures2D.load_factor());
        LOG_TRACE("  m_HashTable_Textures2D.maxload = {}", m_HashTable_Textures2D.max_load_factor());
#endif
        
        return true;
    }
    
    //static
    std::shared_ptr<Texture2D> AssetManager::GetTexture2D(const std::string& logicalName)
    {
        auto found = m_HashTable_Textures2D.find(logicalName);
        
        if (found != m_HashTable_Textures2D.end())
        {
            return found->second.texturePtr;
        }
        
        return nullptr;
    }
    
    //static
    bool AssetManager::DeleteTexture2D(const std::string& logicalName)
    {
        auto found = m_HashTable_Textures2D.find(logicalName);
        
        if (found != m_HashTable_Textures2D.end())
        {
            m_KeysList_Textures2D.erase(m_KeysList_Textures2D.begin() + found->second.keysListIndex);
            m_HashTable_Textures2D.erase(logicalName);
            return true;
        }
        
        return false;
    }
    
    
    
    //static
    bool AssetManager::CreateSprite(const std::string& logicalName,
                                    const std::string& spriteSheetLogicalName,
                                    const SpriteSelector& selector)
    {
#if LOG_PROFILE_HASHTABLES
        LOG_TRACE("Pre-call CreateSprite():");
        LOG_TRACE("  m_HashTable_Sprites.size       = {}", m_HashTable_Sprites.size());
        LOG_TRACE("  m_HashTable_Sprites.buckets    = {}", m_HashTable_Sprites.bucket_count());
        LOG_TRACE("  m_HashTable_Sprites.load       = {}", m_HashTable_Sprites.load_factor());
        LOG_TRACE("  m_HashTable_Sprites.maxload    = {}", m_HashTable_Sprites.max_load_factor());
#endif
        
        /* Get Texture2D from spritesheet logical name */
        std::shared_ptr<Texture2D> texture2D = AssetManager::GetTexture2D(spriteSheetLogicalName);
        if (texture2D == nullptr)
        {
            return false;   // Failed. Stop here.
        }
        
        /* Create Sprite from spritesheet and SpriteSelector */
        std::shared_ptr<Sprite> sprite;
        sprite.reset( Sprite::Create(texture2D, selector.coords, selector.cellSize, selector.spriteSize) );
        
        /* Store Sprite */
        IndexedSprite indexed =
        {
            sprite,
            static_cast<uint32_t>(m_KeysList_Sprites.size()),
            spriteSheetLogicalName
        };
        m_HashTable_Sprites[logicalName] = indexed;
        
        /* Update Keys List */
        m_KeysList_Sprites.emplace_back(logicalName);
        
        /* Store SpriteSelector */
        m_HashTable_SpriteSelectors[logicalName] = selector;
        
#if LOG_PROFILE_HASHTABLES
        LOG_TRACE("Post-call CreateSprite():");
        LOG_TRACE("  m_HashTable_Sprites.size       = {}", m_HashTable_Sprites.size());
        LOG_TRACE("  m_HashTable_Sprites.buckets    = {}", m_HashTable_Sprites.bucket_count());
        LOG_TRACE("  m_HashTable_Sprites.load       = {}", m_HashTable_Sprites.load_factor());
        LOG_TRACE("  m_HashTable_Sprites.maxload    = {}", m_HashTable_Sprites.max_load_factor());
#endif
        
        return true;
    }
    
    //static
    bool AssetManager::UpdateSprite(const std::string& logicalName,
                                    const std::string& spriteSheetLogicalName,
                                    const SpriteSelector& selector)
    {
        auto found = m_HashTable_Sprites.find(logicalName);
        
        if (found != m_HashTable_Sprites.end())
        {
            /* Get Texture2D from spritesheet logical name */
            std::shared_ptr<Texture2D> texture2D = AssetManager::GetTexture2D(spriteSheetLogicalName);
            
            found->second.spritePtr->UpdateData(texture2D, selector.coords, selector.cellSize, selector.spriteSize);
            found->second.spriteSheetName = spriteSheetLogicalName;
            
            /* Update SpriteSelector */
            auto found2 = m_HashTable_SpriteSelectors.find(logicalName);
            
            if (found2 != m_HashTable_SpriteSelectors.end())
            {
                found2->second = selector;
                return true;
            }
        }
        
        return false;
    }
    
    //static
    std::shared_ptr<Sprite> AssetManager::GetSprite(const std::string& logicalName)
    {
        auto found = m_HashTable_Sprites.find(logicalName);
        
        if (found != m_HashTable_Sprites.end())
        {
            return found->second.spritePtr;
        }
        
        return nullptr;
    }
    
    //static
    std::tuple<bool, std::string> AssetManager::GetSpriteSheetName(const std::string& logicalName)
    {
        auto found = m_HashTable_Sprites.find(logicalName);
        
        if (found != m_HashTable_Sprites.end())
        {
            return std::make_tuple(true, found->second.spriteSheetName);
        }
        
        return std::make_tuple(false, "");
    }
    
    //static
    std::tuple<bool, AssetManager::SpriteSelector> AssetManager::GetSpriteSelector(const std::string& logicalName)
    {
        auto found = m_HashTable_SpriteSelectors.find(logicalName);
        
        if (found != m_HashTable_SpriteSelectors.end())
        {
            return std::make_tuple(true, found->second);
        }

        return std::make_tuple(false, found->second);
    }
    
    //static
    bool AssetManager::DeleteSprite(const std::string& logicalName)
    {
        auto found = m_HashTable_Sprites.find(logicalName);
        
        if (found != m_HashTable_Sprites.end())
        {
            m_KeysList_Sprites.erase(m_KeysList_Sprites.begin() + found->second.keysListIndex);
            m_HashTable_Sprites.erase(logicalName);
            m_HashTable_SpriteSelectors.erase(logicalName);
            return true;
        }
        
        return false;
    }
    
}