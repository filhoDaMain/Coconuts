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
#ifndef GNUFILESYSTEM_H
#define GNUFILESYSTEM_H

#include <coconuts/FileSystem.h>

namespace Coconuts
{
    
    namespace {
    namespace Parser
    {
        namespace PATHS
        {
            constexpr auto RUNTIME_CONFDIR_REL_PATH = "";   // empty == same as binary's current location
        }
    }
    } //namespace
    
    
    class GNUFileSystem : public FileSystem
    {
    public:
        GNUFileSystem() = default;
        virtual ~GNUFileSystem() = default;
        
    protected:
        virtual std::string GetCurrDirPathImpl() override;
        virtual std::string GetRuntimeConfDirPathImpl() override;
    };
    
}

#endif /* GNUFILESYSTEM_H */

