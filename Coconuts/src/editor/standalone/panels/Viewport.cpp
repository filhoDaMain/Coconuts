/*
 * Copyright 2021 Andre Temprilho
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

#include "Viewport.h"
#include <coconuts/editor.h>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

namespace Coconuts {
namespace Panels
{
 
    bool Viewport::Init(GameLayer*& gameLayer, std::shared_ptr<Framebuffer> framebuffer)
    {
        m_GameLayerPtr = gameLayer;
        m_IsViewportPanelFocused = false;
        m_FramebufferPtr = framebuffer;
        m_ViewPortTexID = m_FramebufferPtr->GetColorAttachID();
        
        return true;
    }
    
    void Viewport::LiveUpdate()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        ImGui::Begin("Viewport");
        
        /* Change on viewport panel focused state */
        if (m_IsViewportPanelFocused != ImGui::IsWindowFocused())
        {
            m_IsViewportPanelFocused = ImGui::IsWindowFocused();
            
            /* Halt event handling on Game Layer (view port) if not focused */
            m_GameLayerPtr->HaltEvents(!m_IsViewportPanelFocused);
        }
        
        ImVec2 imguiViewportPanelSize = ImGui::GetContentRegionAvail(); // float
        float aspectRatio = (float) (imguiViewportPanelSize.x / imguiViewportPanelSize.y);
        
        /**
         * When View Port Panel changes, update the
         * GameLayer's Framebuffer size
         */
        if (aspectRatio != m_GameLayerPtr->GetActiveSceneAR())
        {
            m_ViewportSize = { imguiViewportPanelSize.x, imguiViewportPanelSize.y };
            m_FramebufferPtr->Resize( m_ViewportSize.x, m_ViewportSize.y );
            m_GameLayerPtr->ChangeViewport(m_ViewportSize.x, m_ViewportSize.y);
            
            /* On some platforms, Framebuffers and its attach's change IDs after Resize() */
            m_ViewPortTexID = m_FramebufferPtr->GetColorAttachID();
        }
        
        ImGui::Image(INT2VOIDP(m_ViewPortTexID), ImVec2{m_ViewportSize.x, m_ViewportSize.y}, ImVec2{0, 1}, ImVec2{1, 0});
        ImGui::End();
        ImGui::PopStyleVar();
    }
    
}
}