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
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <coconuts/cameras/OrthographicCamera.h>
#include <coconuts/time/Timestep.h>
#include <coconuts/EventSystem.h>

namespace Coconuts
{
    
    class CameraController
    {
    public:
        CameraController(OrthographicCamera& camera, float cameraAR_x, float cameraAR_y, float zoomLevel)
            : m_Camera(camera), m_CameraAR_x(cameraAR_x), m_CameraAR_y(cameraAR_y), m_ZoomLevel(zoomLevel)
        {
        }
    
        void OnUpdate(Timestep& ts);
        void OnEvent(Event& event);
    
    private:
        bool OnScrollEvent(InputMouseEvent::MouseScroll& e);
    
        /* Reference to a Camera object */
        OrthographicCamera& m_Camera;
    
        float m_CameraAR_x;
        float m_CameraAR_y;
        float m_ZoomLevel       = 1.0f;
        glm::vec3 m_CameraPos   = {0.0f, 0.0f, 0.0f};
        float m_CameraMoveSpeed = 1.0f;
    };
}

#endif /* CAMERACONTROLLER_H */

