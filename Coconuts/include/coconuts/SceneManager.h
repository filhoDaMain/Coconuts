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
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <coconuts/ecs/Scene.h>
#include <memory>
#include <vector>

namespace Coconuts
{
    
    class SceneManager
    {
    public:
        SceneManager();
        ~SceneManager();
        static SceneManager& GetInstance();
        
        std::shared_ptr<Scene> NewScene(const std::string& name, bool isActive = false);
        std::shared_ptr<Scene> NewScene(uint16_t hardcoded_id, const std::string& name, bool hardcoded_activeState);
        bool DeleteScene(uint16_t id);
        
        std::shared_ptr<Scene> GetScene(uint16_t id);
        bool SetActiveScene(uint16_t id);
        std::shared_ptr<Scene> GetActiveScene();
        
        inline uint16_t GetBufferSize() const { return m_ScenesBuffer.size(); }
        
        bool ClearAll();
        
    private:
        uint16_t NewSceneImpl(const std::string& name, bool isActive = false);
        bool NewSceneImpl(uint16_t hardcoded_id, const std::string& name, bool hardcoded_activeState);
        
    private:
        /* Singleton */
        static SceneManager* s_Instance;
        
        std::vector<std::shared_ptr<Scene>> m_ScenesBuffer;
        uint16_t m_ActiveSceneID;
    };
    
}

#endif /* SCENEMANAGER_H */

