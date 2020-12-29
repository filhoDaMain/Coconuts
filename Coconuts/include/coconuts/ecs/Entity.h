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
#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <utility>
#include <memory>
#include <entityx/entityx.h>
#include <coconuts/ecs/Scene.h>

namespace Coconuts
{
    
    class Entity
    {
    public:
        Entity() = default;
        Entity(std::shared_ptr<Scene> scene);
        ~Entity();
        
        template <typename C, typename ... Args>
        void AddComponent(Args && ... args)
        {
            m_EntityxEntity.assign<C>(std::forward<Args>(args) ...);
        }
        
        template <typename C>
        void RemoveComponent()
        {
            m_EntityxEntity.remove<C>();
        }
        
        template <typename C>
        C& GetComponent()
        {
            return *(m_EntityxEntity.component<C>().get());
        }
        
        template <typename C>
        bool HasComponent()
        {
            return m_EntityxEntity.has_component<C>();
        }
        
        uint64_t GetId() const { return m_EntityxEntity.id().id(); }
        
    private:
        std::shared_ptr<Scene> m_Scene;     /* The Scene this Entity belongs to */
        entityx::Entity m_EntityxEntity;    /* Entity Handler */
        //uint64_t id;
    };
    
}

#endif /* ENTITY_H */

