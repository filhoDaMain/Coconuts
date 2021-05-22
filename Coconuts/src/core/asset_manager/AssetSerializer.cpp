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

#include "AssetSerializer.h"
#include "LoadingRefs.h"
#include <yaml-cpp/yaml.h>
#include <coconuts/Logger.h>

namespace Coconuts
{
    
    namespace {
    namespace Parser
    {
        namespace ROOT
        {
            constexpr auto ROOT_NODE_ASSETMANAGER = "<AssetManager>";
            constexpr auto KEY_SEQ_NODE_TEXTURES2DLIST = "Textures2D List";
            constexpr auto KEY_SEQ_NODE_SPRITESLIST = "Sprites List";
            
            namespace TEXTURE2D
            {
                constexpr auto CLASS_NODE_TEXTURE2D = "<Texture2D>";
                constexpr auto KEY_STR_LOGICALNAME = "logicalName";
                constexpr auto KEY_STR_PATH = "path";
                constexpr auto KEY_SEQ_UI32_SPRITESUSING = "spritesUsing";
            }
            
            namespace SPRITE
            {
                constexpr auto CLASS_NODE_SPRITE = "<Sprite>";
                constexpr auto KEY_STR_LOGICALNAME = "logicalName";
                constexpr auto KEY_STR_SPRITESHEETNAME = "spriteSheetName";
                constexpr auto KEY_UI32_REFERRERINDEX = "referrerIndex";
                constexpr auto NODE_SPRITESELECTOR = "spriteSelector";
                constexpr auto KEY_SEQ_FLOAT_COORDS = "coords";
                constexpr auto KEY_SEQ_FLOAT_CELLSIZE = "cellSize";
                constexpr auto KEY_SEQ_FLOAT_SPRITESIZE = "spriteSize";
            }
        }
    }
    } //namespace
    
    AssetSerializer::AssetSerializer(std::shared_ptr<std::vector<AssetManager::IndexedTexture2D>> indexedTextures,
                                     std::shared_ptr<std::vector<AssetManager::IndexedSprite>> indexedSprites)
    : m_Textures2D(indexedTextures), m_Sprites(indexedSprites)
    {
        //do nothing
    }
    
    
    static void SerializeTexture2D(YAML::Emitter& out, const AssetManager::IndexedTexture2D& htIndex)
    {
        using namespace Parser::ROOT::TEXTURE2D;
        
        /* Get Asset Path from its raw asset it */
        std::string path = LoadingRefs::GetPath(htIndex.assetID);
        
        out << YAML::BeginMap;
        out << YAML::Key << CLASS_NODE_TEXTURE2D;
        out << YAML::BeginMap;
        {
            out << YAML::Key << KEY_STR_LOGICALNAME << YAML::Value << htIndex.logicalName;
            out << YAML::Key << KEY_STR_PATH << YAML::Value << path;
            
            out << YAML::Key << KEY_SEQ_UI32_SPRITESUSING << YAML::Value << YAML::BeginSeq;
            {
                for (auto name : *htIndex.spritesUsing)
                {
                    out << name;
                }
            }
            out << YAML::EndSeq;
        }
        out << YAML::EndMap;
        out << YAML::EndMap;
    }
    
    static void SerializeSprite(YAML::Emitter& out, const AssetManager::IndexedSprite& htIndex)
    {
        using namespace Parser::ROOT::SPRITE;
        
        out << YAML::BeginMap;
        out << YAML::Key << CLASS_NODE_SPRITE;
        out << YAML::BeginMap;
        {
            out << YAML::Key << KEY_STR_LOGICALNAME << YAML::Value << htIndex.logicalName;
            out << YAML::Key << KEY_STR_SPRITESHEETNAME << YAML::Value << htIndex.spriteSheetName;
            out << YAML::Key << KEY_UI32_REFERRERINDEX << YAML::Hex << htIndex.referrerIndex;
            
            out << YAML::Key << NODE_SPRITESELECTOR << YAML::BeginMap;
            {
                out << YAML::Key << KEY_SEQ_FLOAT_COORDS << YAML::Flow << YAML::BeginSeq;
                {
                    out << htIndex.spriteSelector.coords.x;
                    out << htIndex.spriteSelector.coords.y;
                }
                out << YAML::EndSeq;
                
                out << YAML::Key << KEY_SEQ_FLOAT_CELLSIZE << YAML::Flow << YAML::BeginSeq;
                {
                    out << htIndex.spriteSelector.cellSize.x;
                    out << htIndex.spriteSelector.cellSize.y;
                }
                out << YAML::EndSeq;
                
                out << YAML::Key << KEY_SEQ_FLOAT_SPRITESIZE << YAML::Flow << YAML::BeginSeq;
                {
                    out << htIndex.spriteSelector.spriteSize.x;
                    out << htIndex.spriteSelector.spriteSize.y;
                }
                out << YAML::EndSeq;
            }
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
        out << YAML::EndMap;
    }
    
    
    std::string AssetSerializer::Serialize()
    {
        using namespace Parser::ROOT;
        YAML::Emitter out;
        
        out << YAML::BeginMap;
        {
            out << YAML::Key << ROOT_NODE_ASSETMANAGER;
            out << YAML::BeginMap;
            {
                //Textures2D
                out << YAML::Key << KEY_SEQ_NODE_TEXTURES2DLIST << YAML::Value << YAML::BeginSeq;
                for (auto htTexture2D : *m_Textures2D)
                {
                    SerializeTexture2D(out, htTexture2D);
                }
                out << YAML::EndSeq;
                
                /* Sprites */
                out << YAML::Key << KEY_SEQ_NODE_SPRITESLIST << YAML::Value << YAML::BeginSeq;
                for (auto htSprite : *m_Sprites)
                {
                    SerializeSprite(out, htSprite);
                }
                out << YAML::EndSeq;
            }
            out << YAML::EndMap;
        }
        out << YAML::EndMap;
        
        std::string serialized(out.c_str());
        return serialized;
    }
    
    
    
    static bool DeserializeTexture2D(YAML::Node& texture2d_node)
    {
        using namespace Parser::ROOT::TEXTURE2D;
        
        LOG_TRACE("Parsing <Texture2D> ...");
        
        std::string logicalName = texture2d_node[KEY_STR_LOGICALNAME].as<std::string>();
        std::string path = texture2d_node[KEY_STR_PATH].as<std::string>();
        
        LOG_TRACE("  logicalName = {}", logicalName);
        LOG_TRACE("  path = {}", path);
        
        /* Import Texture2D */
        AssetManager::ImportTexture2D(logicalName, path);
        
        return true;
    }
    
    static bool DeserializeSprite(YAML::Node& sprite_node)
    {
        using namespace Parser::ROOT::SPRITE;
        
        LOG_TRACE("Parsing <Sprite> ...");
        
        std::string logicalName = sprite_node[KEY_STR_LOGICALNAME].as<std::string>();
        std::string spriteSheetName = sprite_node[KEY_STR_SPRITESHEETNAME].as<std::string>();
        
        auto spriteselector_node = sprite_node[NODE_SPRITESELECTOR];
        
        auto coords_node = spriteselector_node[KEY_SEQ_FLOAT_COORDS];
        float coords_x = coords_node[0].as<float>();
        float coords_y = coords_node[1].as<float>();
        
        auto cellsize_node = spriteselector_node[KEY_SEQ_FLOAT_CELLSIZE];
        float cellsize_x = cellsize_node[0].as<float>();
        float cellsize_y = cellsize_node[1].as<float>();
        
        auto spritesize_node = spriteselector_node[KEY_SEQ_FLOAT_SPRITESIZE];
        float spritesize_x = spritesize_node[0].as<float>();
        float spritesize_y = spritesize_node[1].as<float>();
        
        LOG_TRACE("  logicalName = {}", logicalName);
        LOG_TRACE("  spriteSheetName = {}", spriteSheetName);
        LOG_TRACE("  spriteSelector:");
        LOG_TRACE("    coords = [ {}, {} ]", coords_x, coords_y);
        LOG_TRACE("    cellSize = [ {}, {} ]", cellsize_x, cellsize_y);
        LOG_TRACE("    spriteSize = [ {}, {} ]", spritesize_x, spritesize_y);
        
        /* Create Sprite */
        AssetManager::SpriteSelector selector;
        selector.coords = {coords_x, coords_y};
        selector.cellSize = {cellsize_x, cellsize_y};
        selector.spriteSize = {spritesize_x, spritesize_y};
        AssetManager::CreateSprite(logicalName, spriteSheetName, selector);
        
        return true;
    }
    
    bool AssetSerializer::Deserialize(std::string& conf)
    {
        using namespace Parser::ROOT;
        
        YAML::Node root = YAML::Load(conf);
        
        auto assetmanager_node = root[ROOT_NODE_ASSETMANAGER];
        if (assetmanager_node)
        {
            LOG_TRACE("Parsing <AssetManager> ...");
            
            auto textures2d_list = assetmanager_node[KEY_SEQ_NODE_TEXTURES2DLIST];
            if (textures2d_list)
            {
                for (auto texture2d : textures2d_list)
                {
                    using namespace Parser::ROOT::TEXTURE2D;
                    auto texture2d_node = texture2d[CLASS_NODE_TEXTURE2D];
                    if (texture2d_node)
                    {
                        DeserializeTexture2D(texture2d_node);
                    }
                }
            }
            
            auto sprites_list = assetmanager_node[KEY_SEQ_NODE_SPRITESLIST];
            if (sprites_list)
            {
                for (auto sprite : sprites_list)
                {
                    using namespace Parser::ROOT::SPRITE;
                    auto sprite_node = sprite[CLASS_NODE_SPRITE];
                    if (sprite_node)
                    {
                        DeserializeSprite(sprite_node);
                    }
                }
            }
        }
        
        return true;
    }
    
}