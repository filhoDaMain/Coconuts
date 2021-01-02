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
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "../GameLayer.h"
#include <coconuts/graphics/Framebuffer.h>

namespace Coconuts {
namespace Panels
{
    class Viewport
    {
    public:
        Viewport() = default;
        bool Init(GameLayer*& gameLayer);
        
        void LiveUpdate();
        
    private:
        /* Pointer to the GameLayer */
        GameLayer* m_GameLayerPtr;
        
        /* Viewport selected flag */
        bool m_IsViewportPanelFocused;
        
        /* GameLayer's Framebuffer */
        std::shared_ptr<Framebuffer> m_Framebuffer;
        
        /* Framebuffer's Color attach texture ID */
        uint32_t m_ViewPortTexID;
        
        /* Viewport panel size */
        glm::vec2 m_ViewportSize;
    };
}
}

#endif /* VIEWPORT_H */

