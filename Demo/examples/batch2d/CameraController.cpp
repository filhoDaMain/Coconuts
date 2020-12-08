#include "CameraController.h"
#include <coconuts/Keyboard.h>
#include <coconuts/Polling.h>

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

