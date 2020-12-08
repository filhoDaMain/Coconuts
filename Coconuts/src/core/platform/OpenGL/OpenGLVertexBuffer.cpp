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

#include "OpenGLVertexBuffer.h"
#include <glad/glad.h>

namespace Coconuts
{
    
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        glGenBuffers(1, &m_RendererID);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        
        glBufferData(GL_ARRAY_BUFFER    /* Vertex Buffer */,
                     size               /* Size (Bytes) of data */,
                     nullptr,
                     GL_DYNAMIC_DRAW    /* Data is expected to be written dynamically */
                    );
    }

    
    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        glGenBuffers(1, &m_RendererID);
        
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        
        glBufferData(GL_ARRAY_BUFFER    /* Vertex Buffer */,
                     size               /* Size (Bytes) of data */,
                     vertices           /* Data to be written in the Vertex Buffer */,
                     GL_STATIC_DRAW     /* Setup once */
                    );
    }
    
    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }
    
    void OpenGLVertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }
    
    void OpenGLVertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
    
}