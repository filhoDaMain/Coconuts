#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <coconuts/cameras/OrthographicCamera.h>
#include <coconuts/time/Timestep.h>

class CameraController
{
public:
    CameraController(Coconuts::OrthographicCamera& camera) : m_Camera(camera)
    {
        
    }
    
    void OnUpdate(Coconuts::Timestep& ts);
    
private:
    /* Reference to a Camera object */
    Coconuts::OrthographicCamera& m_Camera;
    
    glm::vec3 m_CameraPos = {0.0f, 0.0f, 0.0f};
    float m_CameraMoveSpeed = 1.0f;
};

#endif /* CAMERACONTROLLER_H */

