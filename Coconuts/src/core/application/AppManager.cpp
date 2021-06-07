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
#include <fstream>
#include <sstream>

namespace Coconuts
{
    
    ConfigFileTypes ParseFileExtension(const std::string& filepath)
    {
        std::string fileExt = filepath.substr(filepath.find_last_of(".") + 1);
        
        if (fileExt == "ccnproj")
        {
            return ConfigFileTypes::MetaText;
        }
        
        else if (fileExt == "meta")
        {
            return ConfigFileTypes::MetaBinary;
        }
        
        return ConfigFileTypes::Unknown;
        
    }
    
    static bool LoadMetaYAML(const std::string& filepath)
    {
        std::ifstream file(filepath);
        std::stringstream stream;
        stream << file.rdbuf();
        std::string yamlConf = stream.str();
        
        LOG_TRACE("Load Meta YAML");
        LOG_TRACE("{}", yamlConf);
        
        return true;
    }
    
    //static
    bool AppManager::LoadRuntimeConfig(const std::string& filepath)
    {
        //TODO
        
        switch (ParseFileExtension(filepath))
        {
            case ConfigFileTypes::MetaText:
                LoadMetaYAML(filepath);
                break;
                
            case ConfigFileTypes::MetaBinary:
                //TODO
                break;
                
            default:
                //TODO
                break;
        }
        
        return true;
    }
    
}