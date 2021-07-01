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
#include <coconuts/Logger.h>
#include <unistd.h>

namespace Coconuts
{
    
    /* Platform instance */
    FileSystem* FileSystem::s_Instance = new GNUFileSystem();
    
    
    GNUFileSystem::GNUFileSystem()
    :   m_IsInitialized(false),
        m_BinaryPath("nullptr"),
        m_ProcWorkingDirPath("nullptr"),
        m_ConfDirPath("nullptr")
    {
        //empty
    }
    
    bool GNUFileSystem::BootImpl(const char* binary)
    {
        if (m_IsInitialized)
        {
            LOG_ERROR("FileSystem Tools can only be initialized once!");
            return false;
        }
        
        std::string binaryStr(binary);
        
        /* Generate paths */
        this->GenProcWDirPath();
        this->GenBinDirPath(binaryStr);
        this->GenConfDirPath();
        m_IsInitialized = true;
        
        LOG_DEBUG("FileSystem Tools initialized in GNU platform");
        return true;
    }
    
    std::string GNUFileSystem::GetProcWDirPathImpl()
    {
        if (!m_IsInitialized)
        {
            LOG_ERROR("FileSystem was not initialized yet!");
        }
        
        return m_ProcWorkingDirPath;
    }
    
    std::string GNUFileSystem::GetRuntimeBinDirPathImpl()
    {
        if (!m_IsInitialized)
        {
            LOG_ERROR("FileSystem was not initialized yet!");
        }
                
        return m_BinaryPath;
    }
    
    std::string GNUFileSystem::GetRuntimeConfDirPathImpl()
    {
        if (!m_IsInitialized)
        {
            LOG_ERROR("FileSystem was not initialized yet!");
        }
        
        return m_ConfDirPath;
    }
    
    
    //private
    void GNUFileSystem::GenProcWDirPath()
    {
        char buff[128];
        m_ProcWorkingDirPath = ( getcwd(buff, sizeof(buff)) ? std::string(buff) : std::string("nullptr") );
        
        /* Append '/' if needed */
        if (m_ProcWorkingDirPath.back() != '/')
        {
            m_ProcWorkingDirPath += '/';
        }
    }
    
    //private
    void GNUFileSystem::GenBinDirPath(std::string& binary)
    {
        std::string tmp;

        /* Cut binary name from path string */
        tmp = binary.substr(0, binary.find_last_of("/")) + "/";
        
        
        /* (1) Binary path is the same as process working directory */
        if (tmp.compare("./") == 0)
        {
            m_BinaryPath = m_ProcWorkingDirPath;
        }
        
        /* (2) Binary path is already an absolute path */
        else if (tmp.at(0) == '/')
        {
            m_BinaryPath = tmp;
        }
        
        /* (3) Binary path is relative to process working directory path */
        else
        {
            m_BinaryPath = m_ProcWorkingDirPath + tmp;
        }
    }
    
    //private
    void GNUFileSystem::GenConfDirPath()
    {
        m_ConfDirPath = m_BinaryPath + Parser::PATHS::RUNTIME_CONFDIR_REL_PATH;
    }
    
}