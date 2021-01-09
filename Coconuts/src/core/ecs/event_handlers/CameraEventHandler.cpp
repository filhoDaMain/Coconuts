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

#include <coconuts/ecs/event_handlers/CameraEventHandler.h>
#include <coconuts/types.h>
#include <algorithm>

// ECS components
#include <coconuts/ecs/components/OrthoCameraComponent.h>

namespace Coconuts
{
    
    void CameraEventHandler::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        
        /* Dispatch Window Resize Events */
        dispatcher.Dispatch<WindowEvent::WindowResize>
            (BIND_EVENT_FUNCTION(CameraEventHandler::OnWindowResizeEvent));
        
        /* Dispatch Mouse Scroll Events */
        dispatcher.Dispatch<InputMouseEvent::MouseScroll>
            (BIND_EVENT_FUNCTION(CameraEventHandler::OnScrollEvent));
        

    }
    
    // Window Resize Event
    bool CameraEventHandler::OnWindowResizeEvent(WindowEvent::WindowResize& e)
    {                   
        // Retrieve Camera Component
        auto& c_camera = GetComponent<OrthoCameraComponent>();
        
        // Update Aspect Ratio
        float aspectRatio = (float) (e.GetWidth() / e.GetHeight());
        c_camera.aspectRatio = aspectRatio;
        
        // Camera's Projection matrix is updated by Camera Nav System!
        return true;    /* Event was handled. Stop the event propagation */
    }
    
    // Mouse Scroll Event
    bool CameraEventHandler::OnScrollEvent(InputMouseEvent::MouseScroll& e)
    {
        // Retrieve Camera Component
        auto& c_camera = GetComponent<OrthoCameraComponent>();
        
        // Update ZoomLevel
        c_camera.zoomLevel -= (float) ( (float)e.GetOffsetY() * 0.25f);
        c_camera.zoomLevel = std::max<float>(c_camera.zoomLevel, 0.25f);
        
        // Camera's Projection matrix is updated by Camera Nav System!
        return true;    /* Event was handled. Stop the event propagation */
    }
    
    
    void CameraEventHandler::OnCreate()
    {
        
    }
    
    void CameraEventHandler::OnDestroy()
    {
        
    }
    
}