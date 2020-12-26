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
#ifndef EDITORLAYER_H
#define EDITORLAYER_H

#include <coconuts/editor.h>
#include <coconuts/Renderer.h>
#include "GameLayer.h"
#include <glm/glm.hpp>

namespace Coconuts
{
    
    class EditorLayer : public Editor::GUILayer
    {
    public:
        EditorLayer();
        EditorLayer(GameLayer* gameLayer);
        virtual ~EditorLayer() = default;
  
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnPostAttach() override;
        
    private:
        GameLayer* m_GameLayerPtr;
        
        bool m_IsViewportPanelFocused;
        
        /* Renderer2D live statistics */
        Renderer2DStatistics stats;
        
        /* GameLayer's Framebuffer (init during OnPostAttach()) */
        std::shared_ptr<Framebuffer> m_Framebuffer;
        
        /* Framebuffer's Color attach texture ID */
        uint32_t m_ViewPortTexID;
        
        /* GameLayer's CameraController */
        std::shared_ptr<CameraController> m_CameraController;
        
        /* Viewport panel size */
        glm::vec2 m_ViewportSize;
    };
    
}
#endif /* EDITORLAYER_H */

