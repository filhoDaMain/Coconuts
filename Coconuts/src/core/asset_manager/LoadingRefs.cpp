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

#include "LoadingRefs.h"
#include <coconuts/Logger.h>

namespace Coconuts
{
    
    /* Allocate vector */
    std::vector<std::string> LoadingRefs::m_Paths;
    
    
    //static
    uint32_t LoadingRefs::StorePath(const std::string& relPath)
    {
        uint32_t id = (m_Paths.size() - 1);
        m_Paths.emplace_back(relPath);
        
        LOG_TRACE("Loading Refs Stored: Path [ {} ];  ID [ {} ]", relPath, id);
        return id;
    }
    
    //static
    std::string LoadingRefs::GetPath(uint32_t id)
    {
        if (id > m_Paths.size())
        {
            return nullptr;
        }
        
        return m_Paths[id];
    }
    
    //static
    bool LoadingRefs::RemovePath(uint32_t id)
    {
        m_Paths.erase(m_Paths.begin() + id);
        return true;
    }
    
}