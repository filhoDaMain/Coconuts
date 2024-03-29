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
        Scene(uint16_t id, const std::string& name, bool isActive = false);
        ~Scene();
        
        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);
        
        void OnChangeViewport(float x, float y);
        
        entityx::Entity CreateEntity();
        bool DestroyEntity(uint64_t id);
        size_t GetNumberOfEntities() const { return m_EntityManager.entities.size(); }
        std::vector<Entity> GetAllEntities();
        
        bool SetUpdateFlag(bool update = true) { m_IsUpdated = update; return m_IsUpdated;}
        bool SetActiveFlag(bool flag);
        bool IsUpdated() { return m_IsUpdated; }
        bool HaltAllEvents(bool state = true);
        bool HaltEditorCameraNavigation(bool state = true);
        bool DeleteDefaultSceneCamera();
        float GetAspectRatio() { return m_AspectRatio; }
        
        uint16_t GetID() const { return m_ID; }
        std::string GetName() const { return m_Name; }
        bool IsActive() const { return m_IsActive; }
        
    private:        
        uint16_t m_ID;
        std::string m_Name;
        bool m_IsActive;
        bool m_HaltAllEvents;
        bool m_HaltEditorCameraNavigation;
        uint64_t m_DefaultCameraID;
        float m_AspectRatio;
        
        /**
         * When a new Entity is added or modified m_IsUpdated is set to true.
         * It's only reset to false when GetAllEntities() is called.
         */
        bool m_IsUpdated;
        
        entityx::EntityX m_EntityManager;   /* Scene's entities */
        
    private:
        void CreateDefaultSceneCamera();
    };
    
}

#endif /* SCENE_H */

