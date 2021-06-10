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
        static uint16_t NewScene(const std::string& name, bool isActive = false);
        static std::shared_ptr<Scene> GetScene(uint16_t id);
        static bool DeleteScene(uint16_t id);
        
        static bool SetActiveScene(uint16_t id);
        static std::shared_ptr<Scene> GetActiveScene();
        
    private:
        static std::vector<std::shared_ptr<Scene>> m_ScenesBuffer;
        static uint16_t m_ActiveSceneID;
    };
    
}

#endif /* SCENEMANAGER_H */

