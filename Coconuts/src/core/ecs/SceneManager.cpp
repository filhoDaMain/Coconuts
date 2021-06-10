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

#include <coconuts/SceneManager.h>

namespace Coconuts
{
    
    /* Static definitions */
    std::vector<std::shared_ptr<Scene>> SceneManager::m_ScenesBuffer;
    uint16_t SceneManager::m_ActiveSceneID = 0xFFFF;
    
    
    //static
    uint16_t SceneManager::NewScene(const std::string& name, bool isActive)
    {
        uint16_t newID = m_ScenesBuffer.size();
        bool active = m_ActiveSceneID == 0xFFFF ? isActive : false;
        m_ScenesBuffer.push_back(std::make_shared<Scene>(newID, name, active));
        
        if (active)
        {
            m_ActiveSceneID = newID;
        }
        
        return newID;
    }
    
    //static
    std::shared_ptr<Scene> SceneManager::GetScene(uint16_t id)
    {
        if (m_ScenesBuffer.size() <= id)
        {
            return nullptr;
        }
        
        return m_ScenesBuffer[id];
    }
    
    //static
    bool SceneManager::DeleteScene(uint16_t id)
    {
        if (m_ScenesBuffer.size() <= id)
        {
            return false;
        }
        
        m_ScenesBuffer.erase(m_ScenesBuffer.begin() + id);
        return true;
    }
    
    //static
    bool SceneManager::SetActiveScene(uint16_t id)
    {
        if (m_ScenesBuffer.size() <= id)
        {
            return false;
        }
        
        /* Deactivate current active Scene, if any */
        if (m_ActiveSceneID != 0xFFFF)
        {
            m_ScenesBuffer[m_ActiveSceneID]->SetActiveFlag(false);
        }
        
        m_ScenesBuffer[id]->SetActiveFlag(true);
        m_ActiveSceneID = id;
        return true;
    }
    
    //static
    std::shared_ptr<Scene> SceneManager::GetActiveScene()
    {
        if (m_ActiveSceneID == 0xFFFF)
        {
            return nullptr;
        }
        
        return m_ScenesBuffer[m_ActiveSceneID];
    }
    
}