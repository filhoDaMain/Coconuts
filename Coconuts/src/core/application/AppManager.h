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
#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <string>

namespace Coconuts
{
    
    namespace {
    namespace Parser
    {
        namespace FILE_EXTENSIONS
        {
            constexpr auto YAML_PROJECT_FILE_EXT = "ccnproj";
            constexpr auto METABINARY_FILE_EXT = "meta";
        }
    }
    } //namespace
    
    enum class ConfigFileTypes
    {
        Unknown         = 0,
        MetaText        = 1,
        MetaBinary      = 2
    };
    
    class AppManager
    {
    public:
        static bool LoadRuntimeConfig(const std::string& filepath);
    };
    
}

#endif /* APPMANAGER_H */

