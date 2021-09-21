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
        Entity(Scene* scene, const std::string& name = "Untagged");
        ~Entity();
        
        template <typename C, typename ... Args>
        C& AddComponent(Args && ... args)
        {
            auto& cmp = *(m_EntityxEntity.assign<C>(std::forward<Args>(args) ...).get());
            m_Scene->SetUpdateFlag();
            return cmp;
        }
        
        template <typename C>
        void RemoveComponent()
        {
            m_EntityxEntity.remove<C>();
            m_Scene->SetUpdateFlag();
        }
        
        template <typename C>
        C& GetComponent()
        {
            auto& cmp = *(m_EntityxEntity.component<C>().get());
            m_Scene->SetUpdateFlag();
            return cmp;
        }
        
        template <typename C>
        bool HasComponent()
        {
            return m_EntityxEntity.has_component<C>();
        }
        
        uint64_t GetId() const { return m_EntityxEntity.id().id(); }
        
    private:
        Scene*          m_Scene;            /* The Scene this Entity belongs to */
        entityx::Entity m_EntityxEntity;    /* Entity Handler */
        friend class    Scene;
    };
    
}

#endif /* ENTITY_H */

