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
#ifndef LOADINGREFS_H
#define LOADINGREFS_H

#include <string>
#include <vector>

namespace Coconuts
{
    
    class LoadingRefs
    {
    public:
        /**
         * relPath: asset's path relative to assets root folder.
         * Returns asset id.
         */
        static uint32_t StorePath(const std::string& relPath);
        
        /**
         * Returns stored path for the given asset id. 
         */
        static std::string GetPath(uint32_t id);
        
        static bool RemovePath(uint32_t id);
        
    private:
        static std::vector<std::string> m_Paths;
    };
    
}


#endif /* LOADINGREFS_H */

