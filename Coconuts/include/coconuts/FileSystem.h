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
#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>

namespace Coconuts
{
    
    class FileSystem
    {
    public:
        static bool Boot(const char* binary)
        {
            return s_Instance->BootImpl(binary);
        }
        
        /* Get process working directory absolute path */
        inline static std::string GetProcWDirPath()
        {
            return s_Instance->GetProcWDirPathImpl();
        }
        
        /* Get process executable absolute path */
        inline static std::string GetRuntimeBinDirPath()
        {
            return s_Instance->GetRuntimeBinDirPathImpl();
        }
        
        /* Get configuration directory absolute path */
        inline static std::string GetRuntimeConfDirPath()
        {
            return s_Instance->GetRuntimeConfDirPathImpl();
        }
        
    protected:
        /* OS platform dependent implementations */
        virtual bool BootImpl(const char* binary) = 0;
        virtual std::string GetProcWDirPathImpl() = 0;
        virtual std::string GetRuntimeBinDirPathImpl() = 0;
        virtual std::string GetRuntimeConfDirPathImpl() = 0;
        
    private:
        /* Platform defined instance */
        static FileSystem* s_Instance;
    };
    
}

#endif /* FILESYSTEM_H */

