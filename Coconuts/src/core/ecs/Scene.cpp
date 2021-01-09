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
#include <coconuts/ecs/components/EventHandlerComponent.h>

// Default Systems
#include <coconuts/ecs/systems/CameraNavSystem.h>

// Default Event Handlers
#include <coconuts/ecs/event_handlers/CameraEventHandler.h>

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
        
        /* Dispatch Event to Event Handlers */
        m_EntityManager.entities.each<EventHandlerComponent>([&](entityx::Entity thisEntityxEntity, EventHandlerComponent& thisEventHandlerComponent)
        {
            thisEventHandlerComponent.OnEventFunc(thisEventHandlerComponent.instance, e);
        });
    }
    
    void Scene::OnChangeViewport(float x, float y)
    {
        // Change aspect ration on Scene's camera
        m_EntityManager.entities.each<OrthoCameraComponent>([x, y](entityx::Entity thisEntityxEntity, OrthoCameraComponent& thisOrthoCameraComponent)
        {
            thisOrthoCameraComponent.aspectRatio = (float) (x / y);   
            // Cameras matrices are updated by its Camera Nav System!
        });
    }
    
    Scene::Scene()
    {
        CreateSceneCamera();
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
    
    void Scene::CreateSceneCamera()
    {
        Entity camera(this, "Camera");
        camera.AddComponent<OrthoCameraComponent>((float)(16.0f/9.0f), 1.0f).mooveSpeed = 1.0f;
        camera.AddComponent<TransformComponent>().position = {0.0f, 0.0f};
        camera.AddComponent<BehaviorComponent>().AddBehavior<CameraNavSystem>(camera);
        camera.AddComponent<EventHandlerComponent>().AddHandler<CameraEventHandler>(camera);
    }
}