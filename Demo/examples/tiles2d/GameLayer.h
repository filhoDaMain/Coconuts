#ifndef GAMELAYER_H
#define GAMELAYER_H

#include <coconuts/Layer.h>
#include <coconuts/cameras/OrthographicCamera.h>

class GameLayer : public Coconuts::Layer
{
    public:
        GameLayer();
        virtual ~GameLayer() = default;
        
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Coconuts::Timestep ts) override;
        virtual void OnEvent(Coconuts::Event& event) override;
        
    private:
        Coconuts::OrthographicCamera m_Camera;
        
};

#endif /* GAMELAYER_H */

