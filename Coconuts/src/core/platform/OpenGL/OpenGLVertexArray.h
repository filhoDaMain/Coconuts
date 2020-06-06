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
#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H

#include <coconuts/graphics/VertexArray.h>
#include <coconuts/graphics/VertexBuffer.h>
#include <coconuts/graphics/IndexBuffer.h>
#include <memory>

namespace Coconuts
{
    
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();
        
        void Bind() const override;
        void Unbind() const override;
        
        virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer) override;
        
    private:
        uint32_t m_RendererID;
        std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
        std::shared_ptr<IndexBuffer> m_IndexBuffers;
    };
    
}

#endif /* OPENGLVERTEXARRAY_H */

