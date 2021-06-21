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

#include "GNUFileSystem.h"
#include <unistd.h>

namespace Coconuts
{
    
    /* Platform instance */
    FileSystem* FileSystem::s_Instance = new GNUFileSystem();
    
    
    std::string GNUFileSystem::GetCurrDirPathImpl()
    {
        char buff[128];
        return ( getcwd(buff, sizeof(buff)) ? std::string( buff ) : std::string("") );
    }
    
    std::string GNUFileSystem::GetRuntimeConfDirPathImpl()
    {
        std::string abs_path = GNUFileSystem::GetCurrDirPath() + "/" + Parser::PATHS::RUNTIME_CONFDIR_REL_PATH;
        return abs_path;
    }
    
}