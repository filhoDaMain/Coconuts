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

#include <coconuts/cameras/CameraController.h>
#include <coconuts/Keyboard.h>
#include <coconuts/Polling.h>
#include <coconuts/types.h>
#include <coconuts/Logger.h>

namespace Coconuts
{
     
    void CameraController::OnUpdate(Timestep& ts)
    {
        m_DeltaTime = ts;
        
        /**
         * Input polling from Keyboard to 
         * move and rotate Camera
         */

        /* Left */
        if (Polling::IsKeyPressed(Keyboard::KEY_LEFT))
        {
            m_CameraPos.x -= m_CameraMoveSpeed * m_DeltaTime.GetSeconds();
        }

        /* Right */
        if (Polling::IsKeyPressed(Keyboard::KEY_RIGHT))
        {
            m_CameraPos.x += m_CameraMoveSpeed * m_DeltaTime.GetSeconds();
        }

        /* Up */
        if (Polling::IsKeyPressed(Keyboard::KEY_UP))
        {
            m_CameraPos.y += m_CameraMoveSpeed * m_DeltaTime.GetSeconds();
        }

        /* Down */
        if (Polling::IsKeyPressed(Keyboard::KEY_DOWN))
        {
            m_CameraPos.y -= m_CameraMoveSpeed * m_DeltaTime.GetSeconds();
        }


        /* Update Camera's object position */
        m_Camera.SetPosition(m_CameraPos);

        /* Update Camera's object rotation */
        //m_Camera.SetRotation(m_CameraRotation);
    }
    
    void CameraController::ScreenResize(float width, float height)
    {
        m_AspectRatio = (float) (width / height);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    void CameraController::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        /* Dispatch Input Key Press Events */
        dispatcher.Dispatch<InputKeyEvent::KeyPress>
            (BIND_EVENT_FUNCTION(CameraController::OnInputKeyPressEvent));
        
        /* Dispatch Input Key Release Events */
        dispatcher.Dispatch<InputKeyEvent::KeyRelease>
            (BIND_EVENT_FUNCTION(CameraController::OnInputKeyReleaseEvent));
        
        /* Dispatch Mouse Scroll Events */
        dispatcher.Dispatch<InputMouseEvent::MouseScroll>
            (BIND_EVENT_FUNCTION(CameraController::OnScrollEvent));
        
        /* Dispatch Window Resize Events */
        dispatcher.Dispatch<WindowEvent::WindowResize>
            (BIND_EVENT_FUNCTION(CameraController::OnWindowResizeEvent));
    }
    
    
    bool CameraController::OnScrollEvent(InputMouseEvent::MouseScroll& e)
    {
        m_ZoomLevel -= (float) ( (float)e.GetOffsetY() * 0.25f);
        m_ZoomLevel = std::max<float>(m_ZoomLevel, 0.25f);

        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        
        return true;    /* Event was handled. Stop the event propagation */
    }
    
    
    bool CameraController::OnWindowResizeEvent(WindowEvent::WindowResize& e)
    {           
        LOG_DEBUG("Resize");
        //ScreenResize(e.GetWidth(), e.GetHeight());
        return true;    /* Event was handled. Stop the event propagation */
    }
    
    bool CameraController::OnInputKeyPressEvent(InputKeyEvent::KeyPress& e)
    {
        //do nothing
        return true;    /* Event was handled. Stop the event propagation */
    }
    
    bool CameraController::OnInputKeyReleaseEvent(InputKeyEvent::KeyRelease& e)
    {
        //do nothing
        return true;    /* Event was handled. Stop the event propagation */
    }
    
}