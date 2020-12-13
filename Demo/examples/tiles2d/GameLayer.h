#ifndef GAMELAYER_H
#define GAMELAYER_H

#include <memory>
#include <coconuts/Renderer.h>
#include <coconuts/Layer.h>
#include <coconuts/cameras/OrthographicCamera.h>
#include <coconuts/cameras/CameraController.h>

class GameLayer : public Coconuts::Layer
{
    private:
        /* Camera */
        float m_CameraAR_x; // Aspect Ratio X
        float m_CameraAR_y; // Aspect Ratio X
        float m_ZoomLevel;
        Coconuts::OrthographicCamera m_Camera;
        
        /* CameraController */
        Coconuts::CameraController m_CameraController;
        
        /* Sprites */
        std::shared_ptr<Coconuts::Sprite> m_TreeSprite;
        
        /* Spritesheet Texture */
        std::shared_ptr<Coconuts::Texture2D> m_SpritesheetTexture2D;
    
    public:
        GameLayer();
        virtual ~GameLayer() = default;
        
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Coconuts::Timestep ts) override;
        virtual void OnEvent(Coconuts::Event& event) override;        
};

#endif /* GAMELAYER_H */

