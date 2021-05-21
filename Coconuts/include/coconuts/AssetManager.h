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
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <coconuts/graphics/Texture.h>
#include <coconuts/graphics/Sprite.h>
#include <glm/glm.hpp>
#include <tuple>

#define LOG_PROFILE_HASHTABLES   0

namespace Coconuts
{
    
    class AssetSerializer;
    
    class AssetManager
    {
    public:
        struct SpriteSelector
        {
            glm::vec2 coords;
            glm::vec2 cellSize;
            glm::vec2 spriteSize = glm::vec2(1.0f); //by default
        };
        
        struct IndexedTexture2D
        {
            std::shared_ptr<Texture2D>  texturePtr;
            std::string                 logicalName;    // same as key
            uint32_t                    keysListIndex;
            uint32_t                    assetID;
            /* List of Sprites that reference this Texture2D */
            std::shared_ptr<std::vector<std::string>>   spritesUsing;
        };
        
        struct IndexedSprite
        {
            std::shared_ptr<Sprite>     spritePtr;
            std::string                 logicalName;    // same as key
            SpriteSelector              spriteSelector; // human readable selection coordinates
            uint32_t                    keysListIndex;
            std::string                 spriteSheetName;
            /* Index of referred Texture2D::spritesUsing vector pointing to this Sprite */
            uint32_t                    referrerIndex;
        };
        
        struct HashTableDefs
        {
            struct Textures2DHT
            {
                static constexpr uint16_t   reserve = 32;   // reserved buckets
                static constexpr float      max_load_factor = 2.0f;
            };
            
            struct SpritesHT
            {
                static constexpr uint16_t   reserve = 128;  // reserved buckets
                static constexpr float      max_load_factor = 2.0f;
            };
        };
        
    public:
        static void Init(); //non mandatory
        
        static std::string Serialize();
        static bool Deserialize(std::string& conf);
        
        static bool ImportTexture2D(const std::string& logicalName, const std::string& path);
        static std::shared_ptr<Texture2D> GetTexture2D(const std::string& logicalName);
        static std::vector<std::string>& GetAllTexture2DLogicalNames() { return m_KeysList_Textures2D; }
        static bool StoreTexture2D(const std::string& logicalName, std::shared_ptr<Texture2D> texture2D);
        static bool DeleteTexture2D(const std::string& logicalName);
        
        static bool CreateSprite(const std::string& logicalName,
                                 const std::string& spriteSheetLogicalName,
                                 const SpriteSelector& selector);
        static bool UpdateSprite(const std::string& logicalName,
                                 const std::string& spriteSheetLogicalName,
                                 const SpriteSelector& selector);
        static std::shared_ptr<Sprite> GetSprite(const std::string& logicalName);
        static std::tuple<bool, std::string> GetSpriteSheetName(const std::string& logicalName);
        static std::vector<std::string>& GetAllSpriteLogicalNames() { return m_KeysList_Sprites; }
        static std::tuple<bool, SpriteSelector> GetSpriteSelector(const std::string& logicalName);
        static bool DeleteSprite(const std::string& logicalName);
        
    private:
        static uint32_t ReferenceTexture2D(const std::string& texture2DName, const std::string& spriteName);
        static bool EraseReferenceTexture2D(const std::string& texture2DName, uint32_t index);
        
        static IndexedTexture2D GetFullHTIndexedTexture2D(const std::string& name);
        static IndexedSprite GetFullHTIndexedSprite(const std::string& name);
        
        /* HASH TABLES */
        static std::unordered_map<std::string, IndexedTexture2D>    m_HashTable_Textures2D;
        static std::unordered_map<std::string, IndexedSprite>       m_HashTable_Sprites;
        
        /* Keys Lists */
        static std::vector<std::string> m_KeysList_Textures2D;
        static std::vector<std::string> m_KeysList_Sprites;
    };
    
}

#endif /* ASSETMANAGER_H */

