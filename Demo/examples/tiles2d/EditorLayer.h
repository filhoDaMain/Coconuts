#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include <coconuts/editor.h>
#include <coconuts/Renderer.h>

class EditorLayer : public Coconuts::Editor::GUILayer
{
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;
  
        virtual void OnUpdate(Coconuts::Timestep ts) override;  
        
    private:
        /* Renderer2D live statistics */
        Coconuts::Renderer2DStatistics stats;
};


#endif /* EDITORLAYER_H */

