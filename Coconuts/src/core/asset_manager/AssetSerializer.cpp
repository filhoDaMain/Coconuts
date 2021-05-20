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
    
    AssetSerializer::AssetSerializer(std::shared_ptr<std::vector<AssetManager::IndexedTexture2D>> indexedTextures,
                                     std::shared_ptr<std::vector<AssetManager::IndexedSprite>> indexedSprites)
    : m_Textures2D(indexedTextures), m_Sprites(indexedSprites)
    {
        //do nothing
    }
    
    
    static void SerializeTexture2D(YAML::Emitter& out, const AssetManager::IndexedTexture2D& htIndex)
    {
        /* Get Asset Path from its raw asset it */
        std::string path = LoadingRefs::GetPath(htIndex.assetID);
        
        out << YAML::BeginMap;
        out << YAML::Key << "<Texture2D>";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "logicalName" << YAML::Value << htIndex.logicalName;
            out << YAML::Key << "path" << YAML::Value << path;
            
            out << YAML::Key << "spritesUsing" << YAML::Value << YAML::BeginSeq;
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
        out << YAML::BeginMap;
        out << YAML::Key << "<Sprite>";
        out << YAML::BeginMap;
        {
            out << YAML::Key << "logicalName" << YAML::Value << htIndex.logicalName;
            out << YAML::Key << "spriteSheetName" << YAML::Value << htIndex.spriteSheetName;
            out << YAML::Key << "referrerIndex" << YAML::Hex << htIndex.referrerIndex;
            
            out << YAML::Key << "spriteSelector" << YAML::BeginMap;
            {
                out << YAML::Key << "coords" << YAML::Flow << YAML::BeginSeq;
                {
                    out << htIndex.spriteSelector.coords.x;
                    out << htIndex.spriteSelector.coords.y;
                }
                out << YAML::EndSeq;
                
                out << YAML::Key << "cellSize" << YAML::Flow << YAML::BeginSeq;
                {
                    out << htIndex.spriteSelector.cellSize.x;
                    out << htIndex.spriteSelector.cellSize.y;
                }
                out << YAML::EndSeq;
                
                out << YAML::Key << "spriteSize" << YAML::Flow << YAML::BeginSeq;
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
        YAML::Emitter out;
        
        out << YAML::BeginMap;
        {
            out << YAML::Key << "<AssetManager>";
            out << YAML::BeginMap;
            {
                //Textures2D
                out << YAML::Key << "Textures2D List" << YAML::Value << YAML::BeginSeq;
                for (auto htTexture2D : *m_Textures2D)
                {
                    SerializeTexture2D(out, htTexture2D);
                }
                out << YAML::EndSeq;
                
                /* Sprites */
                out << YAML::Key << "Sprites List" << YAML::Value << YAML::BeginSeq;
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
        LOG_TRACE("Parsing <Texture2D> ...");
        
        std::string logicalName = texture2d_node["logicalName"].as<std::string>();
        std::string path = texture2d_node["path"].as<std::string>();
        
        LOG_TRACE("  logicalName = {}", logicalName);
        LOG_TRACE("  path = {}", path);
        
        /* Import Texture2D */
        AssetManager::ImportTexture2D(logicalName, path);
        
        return true;
    }
    
    static bool DeserializeSprite(YAML::Node& sprite_node)
    {
        LOG_TRACE("Parsing <Sprite> ...");
        
        std::string logicalName = sprite_node["logicalName"].as<std::string>();
        std::string spriteSheetName = sprite_node["spriteSheetName"].as<std::string>();
        
        auto spriteselector_node = sprite_node["spriteSelector"];
        
        auto coords_node = spriteselector_node["coords"];
        float coords_x = coords_node[0].as<float>();
        float coords_y = coords_node[1].as<float>();
        
        auto cellsize_node = spriteselector_node["cellSize"];
        float cellsize_x = cellsize_node[0].as<float>();
        float cellsize_y = cellsize_node[1].as<float>();
        
        auto spritesize_node = spriteselector_node["spriteSize"];
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
        YAML::Node root = YAML::Load(conf);
        
        auto assetmanager_node = root["<AssetManager>"];
        if (assetmanager_node)
        {
            LOG_TRACE("Parsing <AssetManager> ...");
            
            auto textures2d_list = assetmanager_node["Textures2D List"];
            if (textures2d_list)
            {
                for (auto texture2d : textures2d_list)
                {
                    auto texture2d_node = texture2d["<Texture2D>"];
                    if (texture2d_node)
                    {
                        DeserializeTexture2D(texture2d_node);
                    }
                }
            }
            
            auto sprites_list = assetmanager_node["Sprites List"];
            if (sprites_list)
            {
                for (auto sprite : sprites_list)
                {
                    auto sprite_node = sprite["<Sprite>"];
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