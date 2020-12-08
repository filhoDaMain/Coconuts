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
#ifndef OPENGLVERTEXBUFFER_H
#define OPENGLVERTEXBUFFER_H

#include <coconuts/graphics/VertexBuffer.h>
#include <stdint.h>
#include <coconuts/graphics/BufferLayout.h>

namespace Coconuts
{
    
    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(uint32_t size);
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();
        
        void Bind() const override;
        void Unbind() const override;
        
        virtual void SetData(const void* data, uint32_t size) override;
        
        void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
        const BufferLayout& GetLayout() const override { return m_Layout; }
        
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };
    
}

#endif /* OPENGLVERTEXBUFFER_H */

