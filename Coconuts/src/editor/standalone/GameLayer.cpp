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

namespace Coconuts
{
    
    std::shared_ptr<Framebuffer> GameLayer::s_Framebuffer;

    void GameLayer::OnUpdate(Timestep ts)
    {
        /* Update Camera Controller */
        m_CameraController.OnUpdate(ts);

        Graphics::LowLevelAPI::SetClearColor({0.0f, 0.0f, 0.0f, 1});
        Graphics::LowLevelAPI::Clear();

        /* Bind Framebuffer */
        s_Framebuffer->Bind();

        /* Set backgound color*/
        Graphics::LowLevelAPI::SetClearColor({0.02f, 0.31f, 0.7f, 1});
        Graphics::LowLevelAPI::Clear();

        /* Render */
        Renderer2D::ResetStatistics();
        Renderer2D::BeginScene(m_Camera);

        Renderer2D::DrawQuad({-0.9f, 1.0f}, {0.8f, 0.8f}, {0.8f, 1.0f, 0.1f, 1.0f});

        Renderer2D::EndScene();

        /* Unbind Framebuffer */
        s_Framebuffer->Unbind();
    }

    void GameLayer::OnEvent(Event& event)
    {
        /* Update CameraController */
        m_CameraController.OnEvent(event);
    }

    GameLayer::GameLayer()
        : m_CameraAR_x(16.0f),
          m_CameraAR_y(9.0f),
          m_ZoomLevel(5.0f),
          m_Camera(-m_CameraAR_x * m_ZoomLevel, -m_CameraAR_x * m_ZoomLevel, -m_CameraAR_y * m_ZoomLevel, m_CameraAR_y * m_ZoomLevel),
          m_CameraController(m_Camera, m_CameraAR_x, m_CameraAR_y, m_ZoomLevel)
    {
        float aspectRatio = (float) (m_CameraAR_x / m_CameraAR_y);
        m_Camera.SetProjection(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    void GameLayer::OnAttach()
    {    
        /* Create Framebuffer */
        FramebufferSpecification spec;
        spec.width = 1280;
        spec.height = 696;

        s_Framebuffer.reset( Framebuffer::Create(spec) );
    }

    void GameLayer::OnDetach()
    {

    }
    
}