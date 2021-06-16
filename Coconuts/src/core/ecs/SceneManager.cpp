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
#include <coconuts/Logger.h>

namespace Coconuts
{
    
    /* Singleton */
    SceneManager* SceneManager::s_Instance = nullptr;
    
    SceneManager::SceneManager()
    :   m_ActiveSceneID(0x0000)
    {
        if (s_Instance != nullptr)
        {
            LOG_ERROR("SceneManager must be a singleton!");
            return;
        }
        
        /* Singleton created */
        s_Instance = this;
        
        LOG_INFO("Singleton instance of SceneManager created");
        
        /* Create dafault Scene and set it to active state */
        s_Instance->NewScene("_Default_", true);
    }
    
    SceneManager::~SceneManager()
    {
        LOG_WARN("Destroy SceneManager");
    }
    
    //static
    SceneManager& SceneManager::GetInstance()
    {
        if (s_Instance == nullptr)
        {
            LOG_WARN("First time SceneManager not initialized!");
            SceneManager(); // updates s_Instance
        }
        
        return *s_Instance;
    }
    
    
    uint16_t SceneManager::NewScene(const std::string& name, bool isActive)
    {
        uint16_t newID = m_ScenesBuffer.size();
        bool active = m_ActiveSceneID == 0x0000 ? isActive : false;
        m_ScenesBuffer.push_back(std::make_shared<Scene>(newID, name, active));
        
        if (active)
        {
            /* Deactivate last active Scene if any */
            if (m_ActiveSceneID != newID)
            {
                m_ScenesBuffer[m_ActiveSceneID]->SetActiveFlag(false);
            }
            
            m_ActiveSceneID = newID;
        }
        
        LOG_TRACE("ScenesBuffer size {}", m_ScenesBuffer.size());
        return newID;
    }
    
    bool SceneManager::NewScene(uint16_t hardcoded_id,
                                const std::string& name,
                                bool hardcoded_activeState)
    {        
        if (hardcoded_id >= m_ScenesBuffer.size())
        {
            m_ScenesBuffer.resize(hardcoded_id + 1);
        }
        
        m_ScenesBuffer[hardcoded_id] = std::make_shared<Scene>(hardcoded_id, name, hardcoded_activeState);
        
        if (hardcoded_activeState)
        {
            /* Deactivate last active Scene if any */
            if (m_ActiveSceneID != hardcoded_id)
            {
                m_ScenesBuffer[m_ActiveSceneID]->SetActiveFlag(false);
            }
            
            m_ActiveSceneID = hardcoded_id;
        }
        
        LOG_TRACE("ScenesBuffer size {}", m_ScenesBuffer.size());
        return true;
    }
    
    std::shared_ptr<Scene> SceneManager::GetScene(uint16_t id)
    {
        if (m_ScenesBuffer.size() <= id)
        {
            return nullptr;
        }
        
        return m_ScenesBuffer[id];
    }
    
    bool SceneManager::DeleteScene(uint16_t id)
    {
        if (m_ScenesBuffer.size() <= id)
        {
            return false;
        }
        
        m_ScenesBuffer.erase(m_ScenesBuffer.begin() + id);
        LOG_WARN("Deleted Scene {}", id);
        return true;
    }
    
    bool SceneManager::SetActiveScene(uint16_t id)
    {
        if (m_ScenesBuffer.size() <= id)
        {
            return false;
        }
        
        /* Deactivate current active Scene */
        m_ScenesBuffer[m_ActiveSceneID]->SetActiveFlag(false);
        
        LOG_TRACE("Active Scene switched from {} to {}", m_ActiveSceneID, id);
        m_ScenesBuffer[id]->SetActiveFlag(true);
        m_ActiveSceneID = id;
        return true;
    }
    
    std::shared_ptr<Scene> SceneManager::GetActiveScene()
    {        
        return m_ScenesBuffer[m_ActiveSceneID];
    }
    
}