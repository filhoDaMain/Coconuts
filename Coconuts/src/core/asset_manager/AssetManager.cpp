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
#include "LoadingRefs.h"
#include "AssetSerializer.h"

namespace Coconuts
{
    
    /* Static Hash Tables Definitions */
    std::unordered_map<std::string, AssetManager::IndexedTexture2D> AssetManager::m_HashTable_Textures2D;
    std::unordered_map<std::string, AssetManager::IndexedSprite>    AssetManager::m_HashTable_Sprites;
    
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
    }
    
    
    //static
    std::string AssetManager::Serialize()
    {
        /* Create a vector of all Indexed Textures */
        std::vector<IndexedTexture2D> allTextures2D;
        for (auto name : GetAllTexture2DLogicalNames())
        {
            allTextures2D.emplace_back( GetFullHTIndexedTexture2D(name) );
        }
        
        /* Create a vector of all Indexed Sprites */
        std::vector<IndexedSprite> allSprites;
        for (auto name : GetAllSpriteLogicalNames())
        {
            allSprites.emplace_back( GetFullHTIndexedSprite(name) );
        }
        
        /* Create serializer and copy data into it */
        AssetSerializer serializer(std::make_shared<std::vector<IndexedTexture2D>>(allTextures2D),
                                   std::make_shared<std::vector<IndexedSprite>>(allSprites));
        
        /* Return serializer output */
        return serializer.Serialize();
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
        
        /* Store Asset Path and get a unique ID */
        uint32_t assetID = LoadingRefs::StorePath(path);
        
        /* Update Texture's ID */
        texture2D->rawID = assetID;
        
        /* Store */
        IndexedTexture2D indexed =
        {
            texture2D,
            logicalName,
            static_cast<uint32_t>(m_KeysList_Textures2D.size()),
            assetID,
            std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>)
        };
        m_HashTable_Textures2D[logicalName] = std::move(indexed);
        
        /* Update Keys List */
        m_KeysList_Textures2D.emplace_back(logicalName);
        
#if LOG_PROFILE_HASHTABLES
        LOG_TRACE("Post-call ImportTexture2D():");
        LOG_TRACE("  m_HashTable_Textures2D.size    = {}", m_HashTable_Textures2D.size());
        LOG_TRACE("  m_HashTable_Textures2D.buckets = {}", m_HashTable_Textures2D.bucket_count());
        LOG_TRACE("  m_HashTable_Textures2D.load    = {}", m_HashTable_Textures2D.load_factor());
        LOG_TRACE("  m_HashTable_Textures2D.maxload = {}", m_HashTable_Textures2D.max_load_factor());
#endif
        
        LOG_DEBUG("Import Texture2D '{}' from file '{}'", logicalName, path);
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
    bool AssetManager::StoreTexture2D(const std::string& logicalName, std::shared_ptr<Texture2D> texture2D)
    {
#if LOG_PROFILE_HASHTABLES
        LOG_TRACE("Pre-call StoreTexture2D():");
        LOG_TRACE("  m_HashTable_Textures2D.size    = {}", m_HashTable_Textures2D.size());
        LOG_TRACE("  m_HashTable_Textures2D.buckets = {}", m_HashTable_Textures2D.bucket_count());
        LOG_TRACE("  m_HashTable_Textures2D.load    = {}", m_HashTable_Textures2D.load_factor());
        LOG_TRACE("  m_HashTable_Textures2D.maxload = {}", m_HashTable_Textures2D.max_load_factor());
#endif
        
        /* Get ID form Texture */
        uint32_t assetID = texture2D->rawID;
        
        /* Store */
        IndexedTexture2D indexed =
        {
            texture2D,
            logicalName,
            static_cast<uint32_t>(m_KeysList_Textures2D.size()),
            assetID,
            std::unique_ptr<std::vector<std::string>>(new std::vector<std::string>)
        };
        m_HashTable_Textures2D[logicalName] = std::move(indexed);
        
        /* Update Keys List */
        m_KeysList_Textures2D.emplace_back(logicalName);
        
#if LOG_PROFILE_HASHTABLES
        LOG_TRACE("Post-call StoreTexture2D():");
        LOG_TRACE("  m_HashTable_Textures2D.size    = {}", m_HashTable_Textures2D.size());
        LOG_TRACE("  m_HashTable_Textures2D.buckets = {}", m_HashTable_Textures2D.bucket_count());
        LOG_TRACE("  m_HashTable_Textures2D.load    = {}", m_HashTable_Textures2D.load_factor());
        LOG_TRACE("  m_HashTable_Textures2D.maxload = {}", m_HashTable_Textures2D.max_load_factor());
#endif
        
        LOG_DEBUG("Store Texture2D '{}'", logicalName);
        return true;
    }
    
    //static
    bool AssetManager::DeleteTexture2D(const std::string& logicalName)
    {
        //TODO  Sould the texture asset ppath be also removed from LoadingRefs?
        //      CAUTION: If StoreTexture2D is called after then it will use a deprecated assetID!
        
        
        auto found = m_HashTable_Textures2D.find(logicalName);
        
        if (found != m_HashTable_Textures2D.end())
        {
            LOG_WARN("Delete Texture2D {}", logicalName);
            
            /* Loop through all referrers and delete them */
            for (std::string spriteName : *(found->second.spritesUsing))
            {
                LOG_TRACE("Also deleting referrer Sprite '{}'", spriteName);
                DeleteSprite(spriteName);
            }
            
            /* Delete entry fom Keys vector */
            m_KeysList_Textures2D.erase(m_KeysList_Textures2D.begin() + found->second.keysListIndex);
            
            /* Update the keysListIndex of Texture2Ds whose keys have shifted positions */
            for (uint32_t keyIndex = found->second.keysListIndex;
                 keyIndex < m_KeysList_Textures2D.size();
                 keyIndex++)
            {
                std::string nameOther = m_KeysList_Textures2D[keyIndex];
                auto findOther = m_HashTable_Textures2D.find(nameOther);
                
                if (findOther != m_HashTable_Textures2D.end())
                {
                    findOther->second.keysListIndex = keyIndex;
                }
            } 
            
            /* Delete entry from hash table */
            m_HashTable_Textures2D.erase(logicalName);
            return true;
        }
        
        return false;
    }
    
    //private satic
    uint32_t AssetManager::ReferenceTexture2D(const std::string& texture2DName, const std::string& spriteName)
    {
        auto found = m_HashTable_Textures2D.find(texture2DName);
        
        if (found != m_HashTable_Textures2D.end())
        {
            found->second.spritesUsing->emplace_back(spriteName);
            return (found->second.spritesUsing->size() - 1);   // return index
        }
        
        return -1;  //0xFFF...
    }
    
    //private static
    bool AssetManager::EraseReferenceTexture2D(const std::string& texture2DName, uint32_t index)
    {
        auto found = m_HashTable_Textures2D.find(texture2DName);
        
        if (found != m_HashTable_Textures2D.end())
        {
            auto iter = found->second.spritesUsing->begin();
            found->second.spritesUsing->erase(iter + index);
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
        
        uint32_t referrerIndex = ReferenceTexture2D(spriteSheetLogicalName, logicalName);
        
        LOG_DEBUG("Create Sprite '{}'", logicalName);
        LOG_TRACE("Assign new hash table ref [{}]->[{}]", logicalName, spriteSheetLogicalName);
        
        /* Store Sprite */
        IndexedSprite indexed =
        {
            sprite,
            logicalName,
            selector,
            static_cast<uint32_t>(m_KeysList_Sprites.size()),
            spriteSheetLogicalName,
            referrerIndex,
        };
        m_HashTable_Sprites[logicalName] = indexed;
        
        /* Update Keys List */
        m_KeysList_Sprites.emplace_back(logicalName);
        
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
            LOG_DEBUG("Update Sprite {}", logicalName);
            
            /* Erase old reference to Texture2D bucket */
            LOG_TRACE("Erase hash table ref [{}]->[{}]", logicalName, found->second.spriteSheetName);
            EraseReferenceTexture2D(found->second.spriteSheetName, found->second.referrerIndex);
            
            /* Get Texture2D from spritesheet logical name */
            std::shared_ptr<Texture2D> texture2D = AssetManager::GetTexture2D(spriteSheetLogicalName);
            
            found->second.spritePtr->UpdateData(texture2D, selector.coords, selector.cellSize, selector.spriteSize);
            found->second.logicalName = logicalName;
            found->second.spriteSelector = selector;
            found->second.spriteSheetName = spriteSheetLogicalName;
            
            /* Assign new reference to a Texture2D bucket */
            LOG_TRACE("Assign new hash table ref [{}]->[{}]", logicalName, spriteSheetLogicalName);
            found->second.referrerIndex = ReferenceTexture2D(spriteSheetLogicalName, logicalName);
            
            return true;
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
        auto found = m_HashTable_Sprites.find(logicalName);
        
        if (found != m_HashTable_Sprites.end())
        {
            return std::make_tuple(true, found->second.spriteSelector);
        }
        
        return std::make_tuple(false, found->second.spriteSelector);
    }
    
    //static
    bool AssetManager::DeleteSprite(const std::string& logicalName)
    {
        auto found = m_HashTable_Sprites.find(logicalName);
        
        if (found != m_HashTable_Sprites.end())
        {
            LOG_WARN("Delete Sprite {}", logicalName);
            
            /* Erase reference to Texture2D bucket */
            LOG_TRACE("Erase hash table ref [{}]->[{}]", logicalName, found->second.spriteSheetName);
            EraseReferenceTexture2D(found->second.spriteSheetName, found->second.referrerIndex);
            
            /* Delete entry from Keys vector */
            m_KeysList_Sprites.erase(m_KeysList_Sprites.begin() + found->second.keysListIndex);
            
            /* Update the keysListIndex of Sprites whose keys have shifted positions */
            for (uint32_t keyIndex = found->second.keysListIndex;
                 keyIndex < m_KeysList_Sprites.size();
                 keyIndex++)
            {
                std::string nameOther = m_KeysList_Sprites[keyIndex];
                auto findOther = m_HashTable_Sprites.find(nameOther);
                
                if (findOther != m_HashTable_Sprites.end())
                {
                    findOther->second.keysListIndex = keyIndex;
                }
            }
            return true;
        }
        
        return false;
    }
    
    //private static
    AssetManager::IndexedTexture2D AssetManager::GetFullHTIndexedTexture2D(const std::string& name)
    {
        IndexedTexture2D copy;  //empty
        
        auto found = m_HashTable_Textures2D.find(name);
        if (found != m_HashTable_Textures2D.end())
        {
            copy = found->second;
        }
        
        return copy;
    }
    
    //private static
    AssetManager::IndexedSprite AssetManager::GetFullHTIndexedSprite(const std::string& name)
    {
        IndexedSprite copy; //empty
        
        auto found = m_HashTable_Sprites.find(name);
        if (found != m_HashTable_Sprites.end())
        {
            copy = found->second;
        }
        
        return copy;
    }
    
}