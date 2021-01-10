/*
 * Copyright 2021 Andre Temprilho
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

#include <coconuts/ecs/systems/CameraNavSystem.h>
#include <coconuts/Keyboard.h>
#include <coconuts/Polling.h>
#include <glm/glm.hpp>

// ECS components
#include <coconuts/ecs/components/OrthoCameraComponent.h>
#include <coconuts/ecs/components/TransformComponent.h>

namespace Coconuts
{
    
    void CameraNavSystem::OnUpdate(Timestep ts)
    {
        /* Component retrieval */
        auto& c_camera = GetComponent<OrthoCameraComponent>();
        auto& c_transform = GetComponent<TransformComponent>();
        
        do
        {
            if (c_camera.halt)
            {
                break;  // jump to Camera's matrices update
            }
            
            /**
             * Input polling from Keyboard to 
             * move and rotate Camera
             */

            /* Left */
            if (Polling::IsKeyPressed(Keyboard::KEY_LEFT))
            {
                c_transform.position.x -= c_camera.mooveSpeed * ts.GetSeconds();
            }

            /* Right */
            if (Polling::IsKeyPressed(Keyboard::KEY_RIGHT))
            {
                c_transform.position.x += c_camera.mooveSpeed * ts.GetSeconds();
            }

            /* Up */
            if (Polling::IsKeyPressed(Keyboard::KEY_UP))
            {
                c_transform.position.y += c_camera.mooveSpeed * ts.GetSeconds();
            }

            /* Down */
            if (Polling::IsKeyPressed(Keyboard::KEY_DOWN))
            {
                c_transform.position.y -= c_camera.mooveSpeed * ts.GetSeconds();
            }
            
        } while (false); //do once
        
        /* Update Camera's Matrices */
        glm::vec3 pos = {c_transform.position.x, c_transform.position.y, 0.0f};
        c_camera.camera.SetPosition(pos);
        c_camera.camera.SetRotation(c_transform.rotationRadians);
        c_camera.camera.SetProjection(-c_camera.aspectRatio * c_camera.zoomLevel, c_camera.aspectRatio * c_camera.zoomLevel, -c_camera.zoomLevel, c_camera.zoomLevel);
    }

    
    void CameraNavSystem::OnCreate()
    {
        
    }
    
    void CameraNavSystem::OnDestroy()
    {
        
    }
    
}