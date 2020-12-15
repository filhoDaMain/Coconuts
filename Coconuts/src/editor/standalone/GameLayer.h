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
#ifndef GAMELAYER_H
#define GAMELAYER_H

#include <memory>
#include <coconuts/Renderer.h>
#include <coconuts/Layer.h>
#include <coconuts/cameras/OrthographicCamera.h>
#include <coconuts/cameras/CameraController.h>

namespace Coconuts
{
    
    class GameLayer : public Layer
    {
        private:
            /* Camera */
            float m_CameraAR_x; // Aspect Ratio X
            float m_CameraAR_y; // Aspect Ratio X
            float m_ZoomLevel;
            OrthographicCamera m_Camera;

            /* CameraController */
            CameraController m_CameraController;

            /* Framebuffer */
            static std::shared_ptr<Framebuffer> s_Framebuffer;

        public:
            GameLayer();
            virtual ~GameLayer() = default;

            virtual void OnAttach() override;
            virtual void OnDetach() override;
            virtual void OnUpdate(Timestep ts) override;
            virtual void OnEvent(Event& event) override;

            static std::shared_ptr<Framebuffer>& GetFramebuffer() { return s_Framebuffer; }
    };
    
}

#endif /* GAMELAYER_H */

