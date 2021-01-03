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
#include <coconuts/ecs/Entity.h>
#include <coconuts/Renderer.h>
#include <coconuts/Logger.h>

// Components
#include <coconuts/ecs/components/TagComponent.h>
#include <coconuts/ecs/components/TransformComponent.h>
#include <coconuts/ecs/components/SpriteComponent.h>
#include <coconuts/ecs/components/OrthoCameraComponent.h>
#include <coconuts/ecs/components/BehaviorComponent.h>


namespace Coconuts
{
    
    void Scene::OnUpdate(Timestep ts)
    {
        /* Systems */
        /* ---------------------------------------------------------------------- */
        
        /* Update Behavior scripts */
        m_EntityManager.entities.each<BehaviorComponent>([&]
        (entityx::Entity thisEntityxEntity, BehaviorComponent& thisBehaviorComponent)
        {  
            thisBehaviorComponent.OnUpdateFunc(thisBehaviorComponent.instance, ts);
        });
        
        
        /* Rendering */
        /* ---------------------------------------------------------------------- */
        
        /* Clear Screen */
        Graphics::LowLevelAPI::SetClearColor({0.0f, 0.0f, 0.0f, 1});
        Graphics::LowLevelAPI::Clear();
        
        /* Reset all Rendering statistics for next draw call */
        Renderer2D::ResetStatistics();
        
        /**
         * NOTE:    We're looping through all entities that have a OrthoCameraComponent,
         *          but there should be only 1 Camera per Scene!!!     
         *
         */
        m_EntityManager.entities.each<OrthoCameraComponent>([&](entityx::Entity thisEntityxEntity, OrthoCameraComponent& thisOrthoCameraComponent)
        {
            
            /* Update Controller */
            if (!m_HaltEditorCameraNavigation)
            {
                thisOrthoCameraComponent.controller.OnUpdate(ts);
            }
            
            /* Begin Scene */
            Renderer2D::BeginScene(thisOrthoCameraComponent.camera);
            
            /* Draw All Sprites on this Scene */
            m_EntityManager.entities.each<TransformComponent, SpriteComponent>([]
            (entityx::Entity thisEntityxEntity, TransformComponent& thisTransformComponent, SpriteComponent& thisSpriteComponent)
            {   
                Renderer2D::DrawRotatedQuad(thisTransformComponent.position,
                                            thisTransformComponent.size,
                                            thisTransformComponent.rotationRadians,
                                            thisSpriteComponent.sprite,
                                            thisSpriteComponent.tilingFactor,
                                            thisSpriteComponent.tintColor);
            });
            
            /* End Scene */
            Renderer2D::EndScene();
        });
        

    }
    
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
    }
    
    Scene::~Scene()
    {    
    }
    
    entityx::Entity Scene::CreateEntity()
    {
        return m_EntityManager.entities.create();
    }
    
    std::vector<Entity> Scene::GetAllEntities()
    {
        std::vector<Entity> all; all.reserve(m_EntityManager.entities.size());
        Scene* self = this;
        
        m_EntityManager.entities.each<TagComponent>([&all, &self]
        (entityx::Entity thisEntityxEntity, TagComponent& tc)
        {  
            Entity tmp;
            tmp.m_EntityxEntity = thisEntityxEntity;
            tmp.m_Scene = self;
            all.emplace_back(tmp);
        });
        
        return all;
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