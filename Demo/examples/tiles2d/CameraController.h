#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <coconuts/cameras/OrthographicCamera.h>
#include <coconuts/time/Timestep.h>
#include <coconuts/EventSystem.h>

class CameraController
{
public:
    CameraController(Coconuts::OrthographicCamera& camera, float cameraAR_x, float cameraAR_y, float zoomLevel)
        : m_Camera(camera), m_CameraAR_x(cameraAR_x), m_CameraAR_y(cameraAR_y), m_ZoomLevel(zoomLevel)
    {
    }
    
    void OnUpdate(Coconuts::Timestep& ts);
    void OnEvent(Coconuts::Event& event);
    
private:
    bool OnScrollEvent(Coconuts::InputMouseEvent::MouseScroll& e);
    
    /* Reference to a Camera object */
    Coconuts::OrthographicCamera& m_Camera;
    
    float m_CameraAR_x;
    float m_CameraAR_y;
    float m_ZoomLevel       = 1.0f;
    glm::vec3 m_CameraPos   = {0.0f, 0.0f, 0.0f};
    
    float m_CameraMoveSpeed = 1.0f;
};

#endif /* CAMERACONTROLLER_H */

