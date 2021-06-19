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

#include "AppManager.h"
#include <coconuts/Logger.h>
#include <coconuts/AssetManager.h>
#include <coconuts/ecs/Serializer.h>
#include <fstream>
#include <sstream>

namespace Coconuts
{
    
    ConfigFileTypes ParseFileExtension(const std::string& filepath)
    {
        std::string fileExt = filepath.substr(filepath.find_last_of(".") + 1);
        
        if (fileExt == Parser::FILE_EXTENSIONS::YAML_PROJECT_FILE_EXT)
        {
            return ConfigFileTypes::MetaText;
        }
        
        else if (fileExt == Parser::FILE_EXTENSIONS::METABINARY_FILE_EXT)
        {
            return ConfigFileTypes::MetaBinary;
        }
        
        return ConfigFileTypes::Unknown;
        
    }
    
    static bool LoadMetaYAML(const std::string& filepath)
    {
        std::ifstream file(filepath);
        
        if (!file.is_open() || file.fail())
        {
            LOG_CRITICAL("Failed to load configuration!");
            LOG_CRITICAL("{} does not exist or failed to open", filepath);
            file.close();
            return false;
        }
        
        std::stringstream stream;
        stream << file.rdbuf();
        std::string yamlConf = stream.str();
        
        /* (1) Import Assets */
        if (!AssetManager::Deserialize(yamlConf))
        {
            LOG_CRITICAL("Failed to import Assets from .ccnproj file!");
            file.close();
            return false;
        }
        
        /* (2) Load Scenes */
        Serializer serializer;
        if (!serializer.Deserialize(yamlConf))
        {
            LOG_CRITICAL("Failed to load Scenes from .ccnproj file!");
            file.close();
            return false;
        }
        
        file.close();
        return true;
    }
    
    static bool LoadMetaBinary(const std::string& filepath)
    {
        LOG_CRITICAL("Loading Meta binary file not supported yet!");
        return false;
    }
    
    //static
    bool AppManager::LoadRuntimeConfig(const std::string& filepath)
    {   
        switch (ParseFileExtension(filepath))
        {
            case ConfigFileTypes::MetaText:
                return LoadMetaYAML(filepath);
                
            case ConfigFileTypes::MetaBinary:
                return LoadMetaBinary(filepath);
                
            default:
                //TODO
                break;
        }
        
        return false;
    }
    
}