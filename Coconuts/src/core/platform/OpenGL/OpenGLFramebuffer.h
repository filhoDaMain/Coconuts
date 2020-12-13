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
#ifndef OPENGLFRAMEBUFFER_H
#define OPENGLFRAMEBUFFER_H

#include <coconuts/graphics/Framebuffer.h>

namespace Coconuts
{
    
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();
        
        virtual void Bind() override;
        virtual void Unbind() override;
        
        virtual const FramebufferSpecification& GetFramebufferSpecification() const override { return m_Spec; }
        virtual uint32_t GetColorAttachID() const override { return m_ColorAttachID; }
        
        void Invalidate();
        
    private:
        FramebufferSpecification m_Spec;
        uint32_t m_RendererID;
        uint32_t m_ColorAttachID;
        uint32_t m_DepthAttachID;
    };
    
}

#endif /* OPENGLFRAMEBUFFER_H */

