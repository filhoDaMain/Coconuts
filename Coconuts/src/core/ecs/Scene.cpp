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
#include <coconuts/ecs/components/OrthoCameraComponent.h>

#include <coconuts/Logger.h>

#include <coconuts/Renderer.h>

namespace Coconuts
{
    
    void Scene::OnUpdate(Timestep ts)
    {
        // Clear Screen
        Graphics::LowLevelAPI::SetClearColor({0.0f, 0.0f, 0.0f, 1});
        Graphics::LowLevelAPI::Clear();
        
        // Reset all Rendering statistics
        Renderer2D::ResetStatistics();
        
        /**
         * NOTE:    We're looping through all entities that have a OrthoCameraComponent,
         *          but there should only be 1 Camera per Scene!!!     
         *
         */
        m_EntityManager.entities.each<OrthoCameraComponent>([&](entityx::Entity thisEntityxEntity, OrthoCameraComponent& thisOrthoCameraComponent)
        {
            
            // Update Controller
            if (!m_HaltEditorCameraNavigation)
            {
                thisOrthoCameraComponent.controller.OnUpdate(ts);
            }
            
            // Begin Scene
            Renderer2D::BeginScene(thisOrthoCameraComponent.camera);
            
            
            Renderer2D::DrawQuad({0.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 1.0f, 0.1f, 1.0f});
            
            // Draw All Sprites for this Scene
            m_EntityManager.entities.each<TagComponent, SpriteComponent>([](entityx::Entity thisEntityxEntity, TagComponent& thisTagComponent, SpriteComponent& thisSpriteComponent)
            {   
                //Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 2.0f}, thisSpriteComponent.sprite, thisSpriteComponent.tilingFactor, thisSpriteComponent.tintColor);
            });
            
            // End Scene
            Renderer2D::EndScene();
        });
        

    }
    
    // Temporary
    void Scene::OnEvent(Event& e)
    {
        if (m_HaltAllEvents)
        {
            return;
        }
        
        // For now, just update CameraController Events, when they exist
        m_EntityManager.entities.each<OrthoCameraComponent>([&](entityx::Entity thisEntityxEntity, OrthoCameraComponent& thisOrthoCameraComponent)
        {
            thisOrthoCameraComponent.controller.OnEvent(e);
        });
    }
    
    void Scene::OnChangeViewport(float x, float y)
    {
        m_EntityManager.entities.each<OrthoCameraComponent>([x, y](entityx::Entity thisEntityxEntity, OrthoCameraComponent& thisOrthoCameraComponent)
        {
            thisOrthoCameraComponent.controller.ScreenResize(x, y);
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
    
    bool Scene::HaltAllEvents(bool state)
    {
        m_HaltAllEvents = state;
        return m_HaltAllEvents;
    }
    
    bool Scene::HaltEditorCameraNavigation(bool state)
    {
        m_HaltEditorCameraNavigation = state;
        return m_HaltEditorCameraNavigation;
    }
}