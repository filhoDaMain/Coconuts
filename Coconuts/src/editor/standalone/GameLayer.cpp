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

#include <string>
#include <coconuts/Renderer.h>
#include "GameLayer.h"


#include <coconuts/ecs/Scene.h>

#include <coconuts/ecs/Entity.h>

#include <coconuts/ecs/components/TransformComponent.h>
#include <coconuts/ecs/components/TagComponent.h>

#include <coconuts/Logger.h>

namespace Coconuts
{

    void GameLayer::OnUpdate(Timestep ts)
    {
        if (!m_HaltEvents)
        {
            /* Update Camera Controller */
            m_CameraController->OnUpdate(ts);   // camera input keys
        }

        Graphics::LowLevelAPI::SetClearColor({0.0f, 0.0f, 0.0f, 1});
        Graphics::LowLevelAPI::Clear();

        /* Bind Framebuffer */
        m_Framebuffer->Bind();

        /* Set backgound color*/
        Graphics::LowLevelAPI::SetClearColor({0.02f, 0.31f, 0.7f, 1});
        Graphics::LowLevelAPI::Clear();

        /* Render */
        Renderer2D::ResetStatistics();
        Renderer2D::BeginScene(m_Camera);

        Renderer2D::DrawQuad({0.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 1.0f, 0.1f, 1.0f});

        Renderer2D::EndScene();

        /* Unbind Framebuffer */
        m_Framebuffer->Unbind();
    }

    void GameLayer::OnEvent(Event& event)
    {
        /* Don't handle any events */
        if (m_HaltEvents)
        {
            return;
        }
        
        /* Update CameraController random events */
        m_CameraController->OnEvent(event);
    }
    
    GameLayer::GameLayer()
    : m_AspectRatio((float) (16.0f/9.0f)),
      m_ZoomLevel(1.0f),
      m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
      m_HaltEvents(false)
      
    {
        m_CameraController = std::make_shared<CameraController>(m_Camera, m_AspectRatio, m_ZoomLevel);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    void GameLayer::OnAttach()
    {   
        LOG_TRACE("Game Layer OnAttach()");
        
        /* Create Framebuffer */
        FramebufferSpecification spec;
        spec.width = 1280.0f;
        spec.height = 696.0f;

        m_Framebuffer.reset( Framebuffer::Create(spec) );
        
        m_ActiveScene = std::make_shared<Scene>();
        
        Entity myEntity(m_ActiveScene);
        
        myEntity.AddComponent<TransformComponent>();
        
        LOG_WARN("Has TransfromComponent ?: {}", myEntity.HasComponent<TransformComponent>() ? "yes" : "no");
        LOG_WARN("Number of entities on scene: {}", m_ActiveScene->GetNumberOfEntities());
        
        myEntity.RemoveComponent<TransformComponent>();
        LOG_WARN("Has TransfromComponent ?: {}", myEntity.HasComponent<TransformComponent>() ? "yes" : "no");
        
        if (myEntity.HasComponent<TagComponent>())
        {
            LOG_WARN("Default tag name: {}", myEntity.GetComponent<TagComponent>().tag);
            
            // Retag
            myEntity.GetComponent<TagComponent>().tag = "Andre";
            LOG_WARN("Re-tagged to: {}", myEntity.GetComponent<TagComponent>().tag);
        }
    }

    void GameLayer::OnDetach()
    {

    }
    
    bool GameLayer::HaltEvents(bool state)
    {
        m_HaltEvents = state;
        return m_HaltEvents;
    }
    
}