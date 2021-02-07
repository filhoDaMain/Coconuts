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

namespace Coconuts
{
    
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
            uint32_t                    keysListIndex;
        };
        
        struct IndexedSprite
        {
            std::shared_ptr<Sprite>     spritePtr;
            uint32_t                    keysListIndex;
        };
        
    public:
        static bool ImportTexture2D(const std::string& logicalName, const std::string& path);
        static std::shared_ptr<Texture2D> GetTexture2D(const std::string& logicalName);
        static std::vector<std::string>& GetAllTexture2DLogicalNames() { return m_KeysList_Textures2D; }
        static bool DeleteTexture2D(const std::string& logicalName);
        
        static bool CreateSprite(const std::string& logicalName,
                                 const std::string& spriteSheetLogicalName,
                                 const SpriteSelector& selector);
        static std::shared_ptr<Sprite> GetSprite(const std::string& logicalName);
        static std::vector<std::string>& GetAllSpriteLogicalNames() { return m_KeysList_Sprites; }
        static std::tuple<bool, SpriteSelector> GetSpriteSelector(const std::string& logicalName);
        static bool DeleteSprite(const std::string& logicalName);
        
    private:
        /* HASH TABLES */
        static std::unordered_map<std::string, IndexedTexture2D>    m_HashTable_Textures2D;
        static std::unordered_map<std::string, IndexedSprite>       m_HashTable_Sprites;
        static std::unordered_map<std::string, SpriteSelector>      m_HashTable_SpriteSlectors;
        
        /* Keys Lists */
        static std::vector<std::string> m_KeysList_Textures2D;
        static std::vector<std::string> m_KeysList_Sprites;
    };
    
}

#endif /* ASSETMANAGER_H */

