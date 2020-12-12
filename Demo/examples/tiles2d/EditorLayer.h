#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include <coconuts/editor.h>

class EditorLayer : public Coconuts::Editor::GUILayer
{
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;
        
        virtual void OnUpdate(Coconuts::Timestep ts) override;            
};


#endif /* EDITORLAYER_H */

