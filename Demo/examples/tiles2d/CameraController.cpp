#include "CameraController.h"
#include <coconuts/Keyboard.h>
#include <coconuts/Polling.h>
#include <coconuts/types.h>
#include <coconuts/Logger.h>

void CameraController::OnUpdate(Coconuts::Timestep& ts)
{
    /**
     * Input polling from Keyboard to 
     * move and rotate Camera
     */
    
    /* Left */
    if (Coconuts::Polling::IsKeyPressed(Coconuts::Keyboard::KEY_LEFT))
    {
        m_CameraPos.x -= m_CameraMoveSpeed * ts.GetSeconds();
    }
    
    /* Right */
    if (Coconuts::Polling::IsKeyPressed(Coconuts::Keyboard::KEY_RIGHT))
    {
        m_CameraPos.x += m_CameraMoveSpeed * ts.GetSeconds();
    }
    
    /* Up */
    if (Coconuts::Polling::IsKeyPressed(Coconuts::Keyboard::KEY_UP))
    {
        m_CameraPos.y += m_CameraMoveSpeed * ts.GetSeconds();
    }
    
    /* Down */
    if (Coconuts::Polling::IsKeyPressed(Coconuts::Keyboard::KEY_DOWN))
    {
        m_CameraPos.y -= m_CameraMoveSpeed * ts.GetSeconds();
    }
    
    
    /* Update Camera's object position */
    m_Camera.SetPosition(m_CameraPos);
    
    /* Update Camera's object rotation */
    //m_Camera.SetRotation(m_CameraRotation);
}

bool CameraController::OnScrollEvent(Coconuts::InputMouseEvent::MouseScroll& e)
{
    m_ZoomLevel -= e.GetOffsetY() * 0.25f;
    m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
    
    float aspectRatio = (float) (m_CameraAR_x / m_CameraAR_y);
    
    m_Camera.SetProjection(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    
    return true;    /* Event was handled. Stop the event propagation */
}

void CameraController::OnEvent(Coconuts::Event& event)
{
    Coconuts::EventDispatcher dispatcher(event);
       
    /* Dispatch Mouse Scroll Events */
    dispatcher.Dispatch<Coconuts::InputMouseEvent::MouseScroll>
        (BIND_EVENT_FUNCTION(CameraController::OnScrollEvent));
}