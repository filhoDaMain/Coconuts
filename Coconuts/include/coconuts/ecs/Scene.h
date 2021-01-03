/*
 * Copyright 2020 Andre Temprilho
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
#ifndef SCENE_H
#define SCENE_H

#include <entityx/entityx.h>
#include <coconuts/time/Timestep.h>
#include <coconuts/EventSystem.h>
#include <vector>

namespace Coconuts
{
    
    // forward declared
    class Entity;
    
    class Scene
    {
    public:
        Scene();
        ~Scene();
        
        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);
        
        void OnChangeViewport(float x, float y);
        
        entityx::Entity CreateEntity();
        size_t GetNumberOfEntities() const { return m_EntityManager.entities.size(); }
        std::vector<Entity> GetAllEntities();
        
        bool HaltAllEvents(bool state = true);
        bool HaltEditorCameraNavigation(bool state = true);
        
    private:
        entityx::EntityX m_EntityManager;
        
        bool m_HaltAllEvents = false;
        bool m_HaltEditorCameraNavigation = false;
    };
    
}

#endif /* SCENE_H */

