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

#include <coconuts/ecs/Scene.h>

#include <coconuts/ecs/components/TransformComponent.h>
#include <coconuts/ecs/components/TagComponent.h>
#include <coconuts/ecs/components/SpriteComponent.h>

#include <coconuts/Logger.h>

#include <coconuts/graphics/Renderer2D.h>

namespace Coconuts
{
    
    void Scene::OnUpdate(Timestep ts)
    {
        m_EntityManager.entities.each<TagComponent, SpriteComponent>([](entityx::Entity thisEntityxEntity, TagComponent& thisTagComponent, SpriteComponent& thisSpriteComponent)
        {
            // Draw
            LOG_INFO("Drawing: {}", thisTagComponent.tag);
            
            Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 2.0f}, thisSpriteComponent.sprite);
        });
    }
    
    Scene::Scene()
    {
#if 0
        entityx::Entity entity = m_Registry.entities.create();
        entityx::Entity entity2 = m_Registry.entities.create();
        
        entity.assign<TransformComponent>(glm::mat4(1.0f));
        entity2.assign<TransformComponent>(glm::mat4(1.0f));
        
        TransformComponent* transform = entity.component<TransformComponent>().get();
        
        LOG_WARN("entity id = {}", entity.id().id());
        LOG_WARN("entity2 id = {}", entity2.id().id());
        
        
        m_Registry.entities.each<TransformComponent>([](entityx::Entity entituxa, TransformComponent &transform)
        {
            // Execute for each entity that has a TransformComponent
            
            LOG_WARN("I'm an entity with a TransformComponent. My ID is {}", entituxa.id().id());
        });
#endif
    }
    
    Scene::~Scene()
    {
        
    }
    
    entityx::Entity Scene::CreateEntity()
    {
        return m_EntityManager.entities.create();
    }
    
}